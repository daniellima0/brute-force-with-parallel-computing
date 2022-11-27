#!/bin/sh

#SBATCH --job-name=C                           # Job name
#SBATCH --nodes=1                              # Run all processes on 2 nodes  
#SBATCH --partition=gpushortc                   # Partition OGBON
#SBATCH --output=out_%j.log                    # Standard output and error log
#SBATCH --ntasks-per-node=1                    # 1 job per node
#SBATCH --account=treinamento                   # Account of the group 

module load gcc/11.1.0
module load cuda/11.8

./cuda zzzzz