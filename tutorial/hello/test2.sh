#!/bin/bash

#PBS -l select=64:ncpus=16:mem=2gb
#PBS -l walltime=0:01:00
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -n 1024 ~/hpc4ds/tutorial/hello/hello
