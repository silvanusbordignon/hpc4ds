#!/bin/bash

#PBS -l select=1:ncpus=1:mem=1gb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -n 1 ~/hpc4ds/mpi/collective/vector-sum/vector-sum 1000 < ~/hpc4ds/mpi/collective/vector-sum/input.txt