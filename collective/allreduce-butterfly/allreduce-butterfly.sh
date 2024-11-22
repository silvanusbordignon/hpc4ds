#PBS -l select=4:ncpus=4:mem=250mb
#PBS -l walltime=0:00:15
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -np 16 ~/hpc4ds/collective/allreduce-butterfly/allreduce-butterfly