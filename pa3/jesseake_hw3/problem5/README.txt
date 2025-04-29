AMATH 583 HOMEWORK 3
========================
Due: 5:00 PM, Friday, April 25, 2025
Written by Jesse Akes
=========================
How to Compile the Shared Object Library: 

- The shared object library is composed of the following files:
    - ref_blas.hpp
    - ref_axpyt.cpp
    - ref_gemt.cpp
    - ref_gemmt.cpp
    - ref_axpyt.hpp
    - ref_gemvt.hpp
    - ref_gemmt.hpp
    - ref_daxpy.hpp
    - ref_dgemv.hpp
    - ref_dgemm.hpp

To compile, enter the commands in the ref_blas directory: 
1. g++ -std=c++17 -c -fPIC ref_axpyt.cpp
2. g++ -std=c++17 -c -fPIC ref_gemvt.cpp
3. g++ -std=c++17 -c -fPIC ref_gemmt.cpp
4. g++ -std=c++17 -c -fPIC ref_daxpy.cpp
5. g++ -std=c++17 -c -fPIC ref_dgemv.cpp
6. g++ -std=c++17 -c -fPIC ref_dgemm.cpp
7. g++ -std=c++17 -shared -o librefBLAS.so ref_axpyt.o ref_gemvt.o ref_gemmt.o ref_daxpy.o ref_dgemv.o ref_dgemm.o

You can then use the shared object libray in your C++ programs by linking to it.
e.g. g++ -std=c++17 tester.cpp -I. -L. -lrefBLAS -o tester
