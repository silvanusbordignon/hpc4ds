#!/bin/bash

#PBS -l select=32:ncpus=32:mem=500mb
#PBS -l walltime=5:30:00
#PBS -q short_cpuQ

#PBS -e max_1024_iter_1000000000_error.txt
#PBS -o max_1024_iter_1000000000_output.txt

~/hpc4ds/openmp/beginner/parallelfor_benchmark/parallelfor_benchmark 1000000000 1 1
~/hpc4ds/openmp/beginner/parallelfor_benchmark/parallelfor_benchmark 1000000000 2 0
~/hpc4ds/openmp/beginner/parallelfor_benchmark/parallelfor_benchmark 1000000000 4 0
~/hpc4ds/openmp/beginner/parallelfor_benchmark/parallelfor_benchmark 1000000000 8 0
~/hpc4ds/openmp/beginner/parallelfor_benchmark/parallelfor_benchmark 1000000000 16 0
~/hpc4ds/openmp/beginner/parallelfor_benchmark/parallelfor_benchmark 1000000000 32 0
~/hpc4ds/openmp/beginner/parallelfor_benchmark/parallelfor_benchmark 1000000000 64 0
~/hpc4ds/openmp/beginner/parallelfor_benchmark/parallelfor_benchmark 1000000000 128 0
~/hpc4ds/openmp/beginner/parallelfor_benchmark/parallelfor_benchmark 1000000000 256 0
~/hpc4ds/openmp/beginner/parallelfor_benchmark/parallelfor_benchmark 1000000000 512 0
~/hpc4ds/openmp/beginner/parallelfor_benchmark/parallelfor_benchmark 1000000000 1024 0