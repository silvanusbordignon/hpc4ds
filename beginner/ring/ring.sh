#!/bin/sh

#PBS -l select=4:ncpus=8:mem=512mb
#PBS -l walltime=0:00:30
#PBS -q short_cpuQ

module load mpich-3.2
mpiexec -n 32 ~/hpc4ds/beginner/ring/ring
