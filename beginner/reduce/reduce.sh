#PBS -l select=4:ncpus=16:mem=500mb
#PBS -l walltime=0:01:00
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -np 64 ~/hpc4ds/beginner/reduce/reduce