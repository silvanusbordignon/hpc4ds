#!/bin/bash

#PBS -l select=32:ncpus=1:mem=500mb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

sleep 5
ls -lrt
echo -n "hostname: " && hostname
