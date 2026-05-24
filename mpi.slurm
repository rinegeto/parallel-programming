#!/bin/bash
#SBATCH --job-name=lab3_mpi
#SBATCH --time=00:20:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4
#SBATCH --mem=8gb
#SBATCH --output=slurm-%j.out
#SBATCH --error=slurm-%j.err
#SBATCH --partition=batch

cd $SLURM_SUBMIT_DIR

echo "Start"
chmod +x mpi_lab
srun ./mpi_lab
echo "End"
