#!/bin/bash
job_prefix=muon50MeV_
NEVENTS=50000
for job in {1..20}
do
    echo "Starting job $job"
    mkdir -p ${job_prefix}job_${job}
    cd ${job_prefix}job_$job
    ln -s ../g4tgeoConfig.C g4tgeoConfig.C
    nohup root -b -q ../runSim.C"(${NEVENTS}, ${job})" >& log_job_${job} &
    cd ..
done
