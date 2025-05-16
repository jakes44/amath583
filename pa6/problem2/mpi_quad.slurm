#!/bin/bash

#SBATCH --job-name=mpi_quad_jesseake

#SBATCH --account=amath
#SBATCH --partition=cpu-g2
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
#SBATCH --mem=5G
#SBATCH --gpus=0
#SBATCH --time=00-00:02:00

#SBATCH --chdir=/gscratch/amath/jesseake/hw6
#SBATCH --export=all
#SBATCH --output=mpi_quad_akes_out.txt
#SBATCH --error=mpi_quad_akes_scaling.csv

# Load the required modules
module load gcc ompi

# Program to run
for i in {1..5}
do
    mpirun -np $((2**i)) ./mpi_quad 100000000
done

for i in {1..7}
do
    mpirun -np 10 ./mpi_quad $((10**i))
done
