#PBS -l select=1:ncpus=2:mem=12GB -l place=scatter:excl
#PBS -l walltime=0:10:00
#PBS -q short_cpuQ
#PBS -o p2p-scatterexcl.csv
#PBS -e p2p-scatterexcl.err

module load mpich-3.2
mpirun.actual -np 2 ~/hpc4ds/p2p/p2p