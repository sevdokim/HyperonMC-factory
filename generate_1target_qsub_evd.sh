#!/bin/bash
#PBS -l ncpus=1
#PBS -l mem=500mb
#PBS -l vmem=1000mb
#PBS -l cput=96:00:00
#PBS -l walltime=96:00:00
#PBS -q alice
#PBS -V

# ^^^ qbatch system requirements.
# This is macro to run 1 thread of simulation. 
# It supposed not to be run directly by user but by other higher-level macro.
# It requires some environment variables to be setted up.
# However  


######################################################################################################
# check if you setted up simulation configuration correctly 
if [ -z $EXTARGET ];    then  export EXTARGET=-1;    fi; 
if [ -z $EXRESON ] ;    then  export EXRESON=-1;     fi;
if [ -z $EXCHANEL ];    then  export EXCHANEL=-1;    fi;
if [ -z $CONTROL ];     then  export CONTROL=0;      fi;
if [ -z $SEED ];        then  export SEED=$(cat /dev/urandom | tr -dc '0-9' | head --bytes 8) ;  fi; # true random 
if [ -z $TARGET ];      then  export TARGET=2;       fi;
if [ -z $EVENTNUMBER ]; then  export EVENTNUMBER=100; fi;
if [ -z $WD ];          then  echo 'you are doing wrong!';  echo 'export WD=|working directory| first';                    return; fi;
if [ -z $MACRODIR ];    then  echo 'you are doing wrong!';  echo 'export MACRODIR=|directory with running macros| first';  return; fi;
if [ -z $FILELIST ];    then  echo 'you are doing wrong!';  echo 'export FILELIST=|path to filelist| first';                  return; fi;
if [ -z $SUFFIX ];      then  echo 'you are doing wrong!';  echo 'export SUFFIX=|name of directory where to generate results| first';                return; fi;
if [ -z $ANCHORS ];     then  echo 'you are doing wrong!';  echo 'export ANCHORS=|directory where you store calibr.cards, h_s_new.dat and coeff_old.dat| first';  return; fi;
if [ -z $REGGEN_CARDS ];then  echo 'you are doing wrong!';  echo 'export REGGEN_CARDS=|/path/to/RegGen.cards| first';  return; fi;
######################################################################################################

#simulation summary
echo 'current WD='$WD
echo 'macro dir is ' $MACRODIR
echo 'suffix which will be used as new directory name is ' $SUFFIX
echo 'filelist to be used: ' $(ls $FILELIST)
echo 'calibr.cards to be used: ' $(ls $ANCHORS/calibr.cards)
echo 'RegGen.cards to be used: ' $(ls $REGGEN_CARDS)
echo 'number of events to be generated ' $EVENTNUMBER
echo 'target medium code is ' $TARGET
echo 'exclusive target is ' $EXTARGET
echo 'exclusive reson is ' $EXRESON
echo 'exclusive chanel is ' $EXCHANEL
echo 'generator control parameter is ' $CONTROL 
echo 'seed = ' $SEED
echo 'starting simulation...'

# VMC environment
source $MACRODIR/vmc_env.sh
LD_LIBRARY_PATH=$MCDIR/lib/tgt_linuxx8664gcc/:$LD_LIBRARY_PATH

cd $WD/$SUFFIX
# copy everything we need
ln -s $REGGEN_CARDS ./RegGen.cards 
cp -u $MACRODIR/run_g3_control.C ./
cp -u $MACRODIR/g3tgeoConfig.C ./
ln -s $MACRODIR/macro/ ./  
ln -s $FILELIST ./file_list.dat
cp -u $ANCHORS/calibr.cards ./
cp -u $ANCHORS/h_s_new.dat ./
cp -u $ANCHORS/coeff_old.dat ./
cp -u $ANCHORS/e_cor_matrix.dat ./
# run MC priduction
if [ $CONVERT_ONLY = yes ] #if this flag is setted then we don't need to perform full simulation, only convert results to Hyperon data format
then
    if [ -e MC_res.dat ] #if MC_res.dat exists then do nothing. otherwise do a full simulation in order to convert something.
    then
	echo 'you asked to skip full simulation and MC_res.dat exists... So skipping...'
    else
	echo 'you asked to skip full simulation. Hovewer MC_res.dat does not exist. So I start full simulation.'
	root -b -q run_g3_control.C\($TARGET,$EVENTNUMBER,\"\",$EXTARGET,$EXRESON,$EXCHANEL,$CONTROL,$SEED\) >& log_production; #do a full simulation
    fi
else
root -b -q run_g3_control.C\($TARGET,$EVENTNUMBER,\"\",$EXTARGET,$EXRESON,$EXCHANEL,$CONTROL,$SEED\) >& log_production; #do a full simulation
fi
ln -s $MACRODIR/converter/exe ./converter  
./converter MC_res.dat $SEED >& log_converter; #convert simulation results into Hyperon data format
cp -u Run${SEED}.gz $MCRUNSDIR/Run${SEED}.gz
chmod a+r $MCRUNSDIR/Run${SEED}.gz
chmod -R a+r .
