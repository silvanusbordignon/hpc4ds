#PBS -l select=2:ncpus=1:mem=250mb
#PBS -l walltime=0:01:00
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -np 2 ~/hpc4ds/beginner/ping-pong-advanced/ping-pong-advanced