#!/bin/bash

#SBATCH --job-name=openblas_dgemm_jesseake

#SBATCH --account=amath
#SBATCH --partition=cpu-g2
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4
#SBATCH --mem=5G
#SBATCH --gpus=0
#SBATCH --time=00-00:15:00

#SBATCH --chdir=/gscratch/amath/jesseake/hw7
#SBATCH --export=all
#SBATCH --output=openblas_dgemm_out.txt
#SBATCH --error=openblas_dgemm_err.txt

# Load the required modules
module load gcc

# Program to run
export PATH=/gscratch/amath/roche-libs/bin:$PATH
export LD_LIBRARY_PATH=/gscratch/amath/roche-libs/lib:$LD_LIBRARY_PATH

#Compile the code
g++ -std=c++14 -c -I/gscratch/amath/roche-libs/include openblas_dgemm.cpp
g++ -std=c++14 -o openblas_dgemm openblas_dgemm.o -L/gscratch/amath/roche-libs/lib -lopenblas

# Run the program
./openblas_dgemm