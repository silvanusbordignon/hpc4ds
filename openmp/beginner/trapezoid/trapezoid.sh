#!/bin/bash

#PBS -l select=1:ncpus=1:mem=500mb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

echo "2.0 6.0 10000" | ~/hpc4ds/openmp/beginner/trapezoid/trapezoid 12 
