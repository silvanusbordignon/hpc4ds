#PBS -l select=4:ncpus=2:mem=256mb
#PBS -l walltime=0:00:15
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -np 8 ~/hpc4ds/mpi/collective/allreduce-butterfly/allreduce-butterfly