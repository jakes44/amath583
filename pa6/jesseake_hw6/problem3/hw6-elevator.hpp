// multiple rider case attempt
/*
Passenger List: elevator maintains list of passengers (pairs of person_id and destination floor)
Batch Processing: elevator picks up as many passengers as possible from the queue if they are on the same floor, respecting the maximum occupancy
Route Management: elevator manages passengers' routes, moving to each passenger's destination floor and logging the necessary information
Complete Logging: detailed logs for each step including entering and exiting the elevator are added to provide comprehensive traceability
also
Occupancy Declaration: occupancy variable initialized to 0 at the start of the elevator function
Occupancy Management: increment occupancy each time a passenger enters the elevator and decrement each time a passenger exits.
*/

#ifndef ELEVATOR_HPP
#define ELEVATOR_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <random>
#include <atomic>
#include <vector>
#include <condition_variable>
#include <tuple>
#include <unordered_map>

using namespace std;

const int NUM_FLOORS = 50;
const int NUM_ELEVATORS = 6;
const int MAX_OCCUPANCY = 5;
const int MAX_WAIT_TIME = 5000; // milliseconds

mutex cout_mtx;
mutex queue_mtx;
condition_variable cv;
queue<tuple<int, int, int>> global_queue; // person_id, start_floor, dest_floor
vector<int> elevator_positions(NUM_ELEVATORS, 0);
atomic<int> num_people_serviced(0);
vector<int> global_passengers_serviced(NUM_ELEVATORS, 0);
int npeople;

void elevator(int id) {
    int occupancy = 0; // Initialize occupancy for each elevator
    vector<pair<int, int>> passengers; // pairs of (person_id, dest_floor)

    while (num_people_serviced.load() < npeople) {
        vector<tuple<int, int, int>> to_pickup;
        {
            unique_lock<mutex> lock(queue_mtx);

            // Wait for passengers or until all people are serviced
            cv.wait(lock, [] { return !global_queue.empty() || num_people_serviced.load() >= npeople; });

            if (num_people_serviced.load() >= npeople) {
                break; // Exit if all people are serviced
            }

            // If there are no passengers in the elevator, move to the next waiting person's start floor
            if (passengers.empty() && !global_queue.empty()) {
                auto [person_id, start_floor, dest_floor] = global_queue.front();
                if (elevator_positions[id] != start_floor) {
                    lock_guard<mutex> cout_lock(cout_mtx);
                    cout << "Elevator " << id << " moving from floor " << elevator_positions[id]
                         << " to floor " << start_floor << endl;
                    elevator_positions[id] = start_floor;
                }
            }

            // Pick up passengers from the queue if at their start floor
            int count = 0;
            queue<tuple<int, int, int>> temp_queue;
            while (!global_queue.empty() && occupancy < MAX_OCCUPANCY) {
                auto [person_id, start_floor, dest_floor] = global_queue.front();
                global_queue.pop();
                if (elevator_positions[id] == start_floor && occupancy < MAX_OCCUPANCY) {
                    to_pickup.push_back({person_id, start_floor, dest_floor});
                    occupancy++;
                } else {
                    temp_queue.push({person_id, start_floor, dest_floor});
                }
            }
            // Restore the remaining people back to the global queue
            while (!temp_queue.empty()) {
                global_queue.push(temp_queue.front());
                temp_queue.pop();
            }
        }

        // Log and add picked up passengers
        for (auto& tup : to_pickup) {
            int person_id = get<0>(tup);
            int dest_floor = get<2>(tup);
            {
                lock_guard<mutex> cout_lock(cout_mtx);
                cout << "Person " << person_id << " entered elevator " << id << endl;
            }
            passengers.emplace_back(person_id, dest_floor);
        }

        // If there are passengers to pick up and elevator is not at their start floor, move there
        if (!to_pickup.empty() && elevator_positions[id] != get<1>(to_pickup.front())) {
            lock_guard<mutex> cout_lock(cout_mtx);
            cout << "Elevator " << id << " moving from floor " << elevator_positions[id]
                 << " to floor " << get<1>(to_pickup.front()) << endl;
            elevator_positions[id] = get<1>(to_pickup.front());
        }

        // If the elevator is not at the next destination, move it there
        if (!passengers.empty() && elevator_positions[id] != passengers.front().second) {
            lock_guard<mutex> cout_lock(cout_mtx);
            cout << "Elevator " << id << " moving from floor " << elevator_positions[id]
                 << " to floor " << passengers.front().second << endl;
            elevator_positions[id] = passengers.front().second;
        }

        // Drop off passengers at their destination floors
        for (auto it = passengers.begin(); it != passengers.end();) {
            int person_id = it->first;
            int dest_floor = it->second;

            if (elevator_positions[id] != dest_floor) {
                lock_guard<mutex> cout_lock(cout_mtx);
                cout << "Elevator " << id << " moving from floor " << elevator_positions[id]
                     << " to floor " << dest_floor << endl;
                elevator_positions[id] = dest_floor;
            }

            {
                lock_guard<mutex> cout_lock(cout_mtx);
                cout << "Person " << person_id << " arrived at floor " << dest_floor << endl;
            }

            it = passengers.erase(it);
            occupancy--;
            global_passengers_serviced[id]++;
            num_people_serviced++;
            cv.notify_all(); // Notify all elevators in case they are waiting
        }
    }

    {
        lock_guard<mutex> lock(cout_mtx);
        cout << "Elevator " << id << " has finished servicing all people." << endl;
        cout << "Elevator " << id << " serviced " << global_passengers_serviced[id] << " passengers." << endl;
    }
}

void person(int id) {
    int curr_floor = rand() % NUM_FLOORS;
    int dest_floor = rand() % NUM_FLOORS;
    while (dest_floor == curr_floor) {
        dest_floor = rand() % NUM_FLOORS;
    }

    {
        lock_guard<mutex> lock(cout_mtx);
        cout << "Person " << id << " wants to go from floor " << curr_floor << " to floor " << dest_floor << endl;
    }

    // Add person to the global queue
    {
        lock_guard<mutex> lock(queue_mtx);
        global_queue.push(make_tuple(id, curr_floor, dest_floor));
    }

    // Notify elevators that a person is waiting
    cv.notify_all();
}
#endif
