#!/bin/bash
job_prefix=py8_with5MeVphoton_
NEVENTS=1000000
for job in {1..20}
do
    echo "Starting job $job"
    mkdir -p ${job_prefix}job_${job}
    cd ${job_prefix}job_$job
    ln -s ../g4tgeoConfig.C g4tgeoConfig.C
    nohup root -b -q ../runSim.C"(${NEVENTS}, ${job})" >& log_job_${job} &
    cd ..
done
