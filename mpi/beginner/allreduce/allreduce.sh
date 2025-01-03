#PBS -l select=2:ncpus=4:mem=250mb
#PBS -l walltime=0:00:30
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -np 8 ~/hpc4ds/mpi/beginner/allreduce/allreduce