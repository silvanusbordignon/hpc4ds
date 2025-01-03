#!/bin/bash

#PBS -l select=64:ncpus=1:mem=10gb
#PBS -l walltime=0:01:00
#PBS -q short_cpuQ
#PBS -o size_64.txt

module load mpich-3.2
mpirun.actual -n 64 ~/hpc4ds/mpi/collective/vector-mat-mult/vector-mat-mult 100000 10000