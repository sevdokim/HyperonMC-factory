#!/bin/bash
#PBS -l ncpus=1
#PBS -l mem=1024mb
#PBS -l cput=48:00:00
#PBS -l walltime=48:00:00
#PBS -q alice
#PBS -V

source /home/sadovsky/HyEnv.sh
cd /data3/users/sadovsky/hyperon/Hyperon_2007_2018_Rev/2008-11
../23.04.2018_prog/calibr.x8664 
