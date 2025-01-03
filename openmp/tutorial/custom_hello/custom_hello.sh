#!/bin/bash

#PBS -l select=1:ncpus=1:mem=500mb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

~/hpc4ds/openmp/tutorial/custom_hello/custom_hello 15
