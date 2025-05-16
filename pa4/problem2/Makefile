test: ref_gemm.cpp ref_gemm.hpp tester.cpp
	g++ -std=c++17 -o test tester.cpp ref_gemm.cpp -I.

timer_0: timer.cpp ref_gemm.cpp
	g++ -std=c++17 -O0 -o timer_0 timer.cpp ref_gemm.cpp -I.

timer_3: timer.cpp ref_gemm.cpp
	g++ -std=c++17 -O3 -o timer_3 timer.cpp ref_gemm.cpp -I.

timer_0_kij: timer_kij.cpp ref_gemm.cpp
	g++ -std=c++17 -O0 -o timer_0_kij timer_kij.cpp ref_gemm.cpp -I.

timer_3_kij: timer_kij.cpp ref_gemm.cpp
	g++ -std=c++17 -O3 -o timer_3_kij timer_kij.cpp ref_gemm.cpp -I.

clean:
	rm test timer_0 timer_3 timer_0_kij timer_3_kij