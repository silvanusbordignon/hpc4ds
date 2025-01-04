#!/bin/bash

#PBS -l select=1:ncpus=64:mem=500mb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

#PBS -o threads_64_real

echo "-10000000000 10000000000 10000000000" | ~/hpc4ds/openmp/beginner/critical_trapezoid/critical_trapezoid 64
