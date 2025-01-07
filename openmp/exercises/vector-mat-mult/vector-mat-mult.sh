#!/bin/bash

#PBS -l select=1:ncpus=32:mem=16GB
#PBS -l walltime=0:10:00
#PBS -q short_cpuQ

~/hpc4ds/openmp/exercises/vector-mat-mult/vector-mat-mult 10 5 1
~/hpc4ds/openmp/exercises/vector-mat-mult/vector-mat-mult 10 5 2
~/hpc4ds/openmp/exercises/vector-mat-mult/vector-mat-mult 10 5 4
~/hpc4ds/openmp/exercises/vector-mat-mult/vector-mat-mult 10 5 8
~/hpc4ds/openmp/exercises/vector-mat-mult/vector-mat-mult 10 5 16
~/hpc4ds/openmp/exercises/vector-mat-mult/vector-mat-mult 10 5 32
