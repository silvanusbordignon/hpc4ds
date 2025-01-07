#!/bin/bash

#PBS -l select=32:ncpus=64:mem=500mb
#PBS -l walltime=0:15:00
#PBS -q short_cpuQ

#PBS -o iter_1000000000000_threads_2048
~/hpc4ds/openmp/exercises/estimating_pi/estimating_pi 1000000000000 2048
