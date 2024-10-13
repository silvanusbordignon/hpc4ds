#!/bin/bash

#PBS -l select=1:ncpus=5:mem=2gb
#PBS -l walltime=0:00:15
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -n 5 ~/hpc4ds/tutorial/message-chain/message-chain
