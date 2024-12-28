# HPC4DS

Repository collecting exercises from the High Performance Computing for Data Science course. This course is offered by prof. Sandro Fiore at the University of Trento, and aims at teaching how we can apply parallel computing principles to develop programs able to scale on large, high performance machines.

## Info

The university cluster uses [PBS](https://altairengineering.it/pbs-professional/) as the resource manager, meaning the scripts that will be ran on each chunk will include PBS directives, managing the resources required and the queue we're submitting the job into.

The MPI version we'll be working with is [mpich](https://www.mpich.org), the message passing interface implementation by MIT. The version running on the cluster is the 3.2, and it will be loaded as a module.

For each exercise we write a script containing the PBS directives and the commands we run on each cluster, the source file from which we'll compile our software, and the executable that will be launched by the script; it can be a shell script or a python script.

## Commands

We compile files by loading the `mpich` module and launching the `mpicc` command (the `-lm` links the cmath library):
```sh
module load mpich-3.2
mpicc -g -Wall -std=c99 -pedantic -o file_name file_name.c [-lm]
```

We submit the scripts to be ran on each chunk using the `qsub` command:
```sh
qsub script.[sh|py]
```
