#!/bin/bash

#SBATCH --job-name=mpi_broadcast_jesseake

#SBATCH --account=amath
#SBATCH --partition=cpu-g2
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
#SBATCH --mem=5G
#SBATCH --gpus=0
#SBATCH --time=00-00:02:00

#SBATCH --chdir=/gscratch/amath/jesseake/hw6
#SBATCH --export=all
#SBATCH --output=mpi_broadcast_akes_out.txt
#SBATCH --error=mpi_broadcast_akes_scaling.csv

# Load the required modules
module load gcc ompi

# Program to run
for i in {3..8}
do
    mpirun -np 4 ./mpi_broadcast  $((2**i))
done

for i in {3..8}
do
    mpirun -np 32 ./mpi_broadcast  $((2**i))
done

for i in {3..8}
do
    mpirun -np 4 ./mpi_broadcast_good  $((2**i))
done

for i in {3..8}
do
    mpirun -np 32 ./mpi_broadcast_good  $((2**i))
done
