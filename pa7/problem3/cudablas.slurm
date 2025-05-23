#!/bin/bash

#SBATCH --job-name=cudablas_jesseake

#SBATCH --account=amath
#SBATCH --partition=gpu-rtx6k
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=5G
#SBATCH --gpus=1
#SBATCH --time=00-00:15:00

#SBATCH --chdir=/gscratch/amath/jesseake/hw7/problem3
#SBATCH --export=all
#SBATCH --output=cudablas_out.txt
#SBATCH --error=cudablas_err.txt

# Load the required modules
module load gcc cuda

# Program to run
#Compile the code
cp cudablas.cpp cudablas.cu
nvcc -c -arch sm_75  cudablas.cu 
c++ -o cudablas cudablas.o -L/mmfs1/sw/cuda/12.2.2/lib64/ -lcudart -lcublas -lm

# Run the program
srun -n 1 --gpus=1 ./cudablas