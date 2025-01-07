#!/bin/bash

#PBS -l select=16:ncpus=16:mem=500mb
#PBS -l walltime=0:10:00
#PBS -q short_cpuQ

~/hpc4ds/openmp/exercises/custom_reduction/custom_reduction 1 100000000
~/hpc4ds/openmp/exercises/custom_reduction/custom_reduction 2 100000000
~/hpc4ds/openmp/exercises/custom_reduction/custom_reduction 4 100000000
~/hpc4ds/openmp/exercises/custom_reduction/custom_reduction 8 100000000
~/hpc4ds/openmp/exercises/custom_reduction/custom_reduction 16 100000000
