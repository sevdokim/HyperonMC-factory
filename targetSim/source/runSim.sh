#!/bin/bash

NEVENTS=1000000
for job in {0..1}
do
    echo "Starting job $job"
    mkdir -p job_${job}
    cd job_$job
    ln -s ../g4tgeoConfig.C g4tgeoConfig.C
    nohup root -b -q ../runSim.C"(${NEVENTS}, ${job})" >& log_job_${job} &
    cd ..
done
