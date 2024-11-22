#!/bin/bash

#PBS -l select=1:ncpus=2:mem=1gb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -n 2 ~/hpc4ds/collective/trapezoidal/trapezoidal