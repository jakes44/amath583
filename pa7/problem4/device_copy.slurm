#!/bin/bash

#SBATCH --job-name=devicecopy_jesseake

#SBATCH --account=amath
#SBATCH --partition=gpu-rtx6k
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=10G
#SBATCH --gpus=1
#SBATCH --time=00-00:10:00

#SBATCH --chdir=/gscratch/amath/jesseake/hw7/problem4
#SBATCH --export=all
#SBATCH --output=devicecopy_out.txt
#SBATCH --error=devicecopy_err.txt

# Load the required modules
module load gcc cuda

# Program to run
#Compile the code
cp device_copy.cpp device_copy.cu
nvcc -c -arch sm_75  device_copy.cu 
g++ -o device_copy device_copy.o -L/mmfs1/sw/cuda/12.2.2/lib64/ -lcudart -lm

# Run the program
srun -n 1 --gpus=1 ./device_copy