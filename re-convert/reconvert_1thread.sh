#!/bin/bash
#PBS -l ncpus=1
#PBS -l vmem=1000mb
#PBS -l cput=02:00:00
#PBS -l walltime=12:00:00
#PBS -q ihep-short
#PBS -V


THIS_THREAD_PATH=$PBS_O_WORKDIR
if [ ! -z $THIS_THREAD_PATH ] ; then
    if cd $THIS_THREAD_PATH ; then echo "I cd-ed in $(pwd)" ; fi 
fi

if [ -f reconvert_threads.parameters ] ; then
    echo "reconvert_threads.parameters exists so starting reconvertion"
    reconvert_threads $(cat reconvert_threads.parameters)
else
    echo "did not find reconvert_threads.parameters in $THIS_THREAD_PATH"
    echo "probably you are doing wrong"
fi
