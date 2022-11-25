# brute-force-with-parallel-computing
Improving a brute force code using HPC resources and parallel computing API's like CUDA, OpenMPI and OpenMP.

```bash
clear; gcc brute-force.c -o sequential; ./sequential senha

clear; nvcc brute-force.cu -o cuda; ./cuda senha

clear; gcc brute-force-openmp.c -o openmp -fopenmp; ./openmp senhatez 4

```