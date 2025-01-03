# TODO: 
# - update resouces, consider RAM usage if data size increases
# - make it into a loop to run the different processes with varying resources

#PBS -l select=1:ncpus=2:mem=1GB
#PBS -l walltime=0:01:00
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -np 2 ~/hpc4ds/mpi/p2p/p2p