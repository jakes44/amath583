mpi_broadcast: broadcast.cpp my_broadcast.hpp
	mpic++ -std=c++17 -o mpi_broadcast broadcast.cpp -I.

mpi_broadcast_good: broadcast_no.cpp
	mpic++ -std=c++17 -o mpi_broadcast_good broadcast_no.cpp -I.

clean:
	rm -f broadcast