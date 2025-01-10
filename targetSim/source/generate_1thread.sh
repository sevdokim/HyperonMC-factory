#!/bin/bash
#PBS -l ncpus=1
#PBS -l vmem=1000mb
#PBS -l mem=1000mb
#PBS -V

# ^^^ qbatch system requirements.
# This is macro to run 1 thread of simulation. 
# It supposed not to be run directly by user but by other higher-level macro.
# It requires some environment variables to be setted up.
# However  
echo "job started " $(date)
# Read config of the simulation
THIS_THREAD_PATH=$PBS_O_WORKDIR
if [ ! -z $THIS_THREAD_PATH ] ; then
    THIS_THREAD_PATH=.
fi
if ls $THIS_THREAD_PATH/shell_env.sh >& /dev/null ; then
    for var in WD MCDIR SUFFIX PRODUCTION_NAME UNIC_CODE LD_LIBRARY_PATH SEED NTHREADS PATH PWD EVENTNUMBER MACRODIR TARGET PHOTON_ENERGY SEED TTHICKNESS TRADIUS USEPYTHIA
    do
	for val in $(grep "$var=" $THIS_THREAD_PATH/shell_env.sh) ; do
	    if [ ! -z $val ] ; then 
		export $val
		echo 'Using earlier defined variable ' $val
	    fi
	done
    done
fi
######################################################################################################
# check if you setted up simulation configuration correctly
# 
# default values for simulation config if not setted up 
if [ -z $EVENTNUMBER ];  then  export EVENTNUMBER=100;      fi;
if [ -z $PHOTON_ENERGY ];then  export PHOTON_ENERGY=0.0044; fi;
if [ -z $SEED ];         then  export SEED=0;               fi;
if [ -z $TARGET ];       then  export TARGET=1;             fi;
if [ -z $TTHICKNESS ];   then  export TTHICKNESS=6;         fi;
if [ -z $TRADIUS ];      then  export TRADIUS=2;            fi;
if [ -z $USEPYTHIA ];    then  export USEPYTHIA=1;          fi;



#
# without those variables simulation is not possible
if [ -z $WD ];          then  
    echo 'you are doing wrong!';  
    echo 'export WD=/path/to/production/directory/'
    echo 'first! then try again.';
    return; fi;
if [ -z $MACRODIR ];    then  
    echo 'you are doing wrong!';  
    echo 'export MACRODIR=/directory/with/running/macros/'
    echo 'first! then try again.';  
    return; fi;
if [ -z $SUFFIX ];      then  
    echo 'you are doing wrong!';  
    echo 'export SUFFIX=|name of directory where to generate results|'
    echo 'first! then try again.';
    return; fi;
######################################################################################################

#simulation summary
echo 'current WD='$WD
echo 'macro dir is ' $MACRODIR
echo 'suffix which will be used as new directory name is ' $SUFFIX
echo 'number of events to be generated ' $EVENTNUMBER
echo 'target medium code is ' $TARGET
echo 'seed = ' $SEED
echo 'starting simulation...'

# VMC environment
source $MACRODIR/env.sh
LD_LIBRARY_PATH=$MACRODIR/../install/lib/:$LD_LIBRARY_PATH
mkdir -p $scratch/$WD/$SUFFIX
cd $scratch/$WD/$SUFFIX
echo "I cd'ed into " $(pwd)
# copy everything we need
ln -s $MACRODIR/run_g4.C ./run_g4.C
ln -s $MACRODIR/load_g4.C ./load_g4.C
ln -s $MACRODIR/g4tgeoConfig.C ./g4tgeoConfig.C
#ln -s $MACRODIR/py8Config.cfg .
ln -s $MACRODIR/macro/ ./macro  

# run MC production
root -b -q load_g4.C run_g4.C\($EVENTNUMBER,$SEED,$PHOTON_ENERGY,$TARGET,$TRADIUS,$TTHICKNESS,$USEPYTHIA\) >& log_production; #do a full simulation
echo 'First 700 strings and last 700 strings of log_production. For full file look MC_res.dat.bz2 ' > log_production_part
head -n 700 log_production >> log_production_part
echo '.................................................................' >> log_production_part
echo '.............cutted here.........................................' >> log_production_part
echo '.................................................................' >> log_production_part
tail -n 700 log_production >> log_production_part
bzip2 -z9 log_production

mkdir -p $WD/histos
mv histos.root $WD/histos/${SUFFIX}.root

echo "I produced new production files, cp log_production to $WD/$SUFFIX"
#copy results to $WD/$SUFFIX if convert_only != yes
if [ ! -z $scratch ] ; then
    mkdir -p $WD/$SUFFIX
    cp -a * $WD/$SUFFIX
fi

echo "job ended " $(date)
