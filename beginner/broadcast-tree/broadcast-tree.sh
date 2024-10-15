#PBS -l select=16:ncpus=16:mem=250mb
#PBS -l walltime=0:00:15
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -np 256 ~/hpc4ds/beginner/broadcast-tree/broadcast-tree