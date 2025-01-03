#!/bin/bash

#PBS -l select=16:ncpus=16:mem=500mb
#PBS -l walltime=0:00:30
#PBS -q short_cpuQ

#PBS -o theads_256

echo "-10000000000 10000000000 10000000000" | ~/hpc4ds/openmp/beginner/reduction_trapezoid/reduction_trapezoid 256
