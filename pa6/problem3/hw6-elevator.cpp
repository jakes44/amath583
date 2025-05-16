#include "hw6-elevator.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " npeople" << endl;
        return 1;
    }
    npeople = atoi(argv[1]);
    if (npeople > 7500) {
        cerr << "Number of people exceeds maximum limit of 7500" << endl;
        return 1;
    }

    thread elevators[NUM_ELEVATORS];
    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        elevators[i] = thread(elevator, i);
    }

    default_random_engine gen;
    uniform_int_distribution<int> dist(0, MAX_WAIT_TIME);

    for (int i = 0; i < npeople; ++i) {
        int wait_time = dist(gen);
        this_thread::sleep_for(chrono::milliseconds(wait_time));
        thread(person, i).detach();
    }

    for (auto &e : elevators) {
        e.join();
    }

    cout << "Job completed!" << endl;
    int total_passengers_serviced = 0;
    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        total_passengers_serviced += global_passengers_serviced[i];
    }
    cout << "Total passengers serviced by all elevators: " << total_passengers_serviced << endl << flush;

    return 0;
}

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