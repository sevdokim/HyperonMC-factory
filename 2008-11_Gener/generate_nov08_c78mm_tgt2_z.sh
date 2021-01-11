#
#   Hyperon RegGen event generator macro
# ----------------------------------------
#
# Hyperon PERIODs: 2007-11 2008-04 2008-11 2009-11 2011-04 2011-11 2012-04 2012-11
#                  2013-03 2014-11 2015-03 2015-11 2016-11 2017-12 2018-03
# Periods EvntTot: 
# 
# 2007-11 TARGETs: be79mm c78mm al35mm cu7mm pb3mm ch80mm
#         TGevent:
# 2008-04 TARGETs: be79mm c78mm cu7mm 
#         TGevent:
# 2008-11 TARGETs: be79mm c78mm al35mm cu7mm sn5mm pb3mm ch80mm
#         TGevent:
#...............................................................................................................
#     Input parameters:
#===========================
#
if [ $HYMC_CONFIG_DEFINED = yes ]
then
    unset HYMC_CONFIG_DEFINED
    echo 'Using predefined config!'
else
    export EVENTNUMBER=100000                     # Total event number to be generated per 1 production thread
#   export NTHREADS=4                             # total number of production threads
    export NTHREADS=1                             # total number of production threads
    export PERIOD=2008-11                         # Hyperon Runs (2007-11, 2008-04, 2008-11, 2009-11, 2011-04 ... -- 15 runs in total) 
    export PERIOD_PRFX=nov08_                     # Period prefix for files:  file_list.dat ==> file_list_nov08.dat
    export TGT_PRFX=c78mm                         # Hyperon Tgts dlya formirovaniya imeni file_list_nov08.dat ==> file_list_nov08_be79mm.dat
                                                  # TGT_PRFX = be79mm, c78mm, al35mm, al17mm, cu3mm, cu7mm, sn5mm, pb3mm, ch80mm
    export PRODUCTION_NAME=$PERIOD_PRFX$TGT_PRFX  # production name
    export CONVERT_ONLY=no                        # yes or no
fi
#
export MCDIR=/data/users/sadovsky/hyperon/Hyperon_2007_2018_Rev/MC_05.02.2019/ #name of top MC directory
export REGGEN_CARDS=$MCDIR/${PERIOD}_Gener/RegGen_$PRODUCTION_NAME.cards        #name of RegGen.cards
export MACRODIR=$MCDIR/Hyp_RegGen/                                              #name of MC tool directory
export ANDIR=/data/users/sadovsky/hyperon/Hyperon_2007_2018_Rev/               #name of top analysis directory
export MCRUNSDIR=/data/HYPERON_MC_data/$PERIOD
mkdir -p $MCRUNSDIR
export WD=$MCDIR/${PERIOD}_Gener/$PRODUCTION_NAME
export ANCHORS=$MCDIR/${PERIOD}_Gener/
export FILELIST=$MCDIR/${PERIOD}_Gener/file_lists/file_list_$PERIOD_PRFX$TGT_PRFX.dat
#
export TARGET=10              # Hyperon Tgts for GEANT (2=Be, 10=C, 3=Al35mm, 4=Al17mm, 5=Cu3mm, 9=Cu7, 8=Sn, 7=Pb, 11=ch80) 
export EXTARGET=3             # All exclusive targets of Hyperon setup: 1=S4, 2=target, 3=Sa, 0=all of them
#
#export PROBABILITY_S4=0.001  # S4  interaction probability 
#export PROBABILITY_TARG=0.99 # TRG interaction probability
#export PROBABILITY_SA=0.009  # SA  interaction probability
export DELTA_S4=-0            # S4  shift in mm relative to the nominal position
#export THICKNESS_S4=2        # S4  thickness in mm
export DELTA_SA=-361           # SA  shift in mm relative to the nominal position
export DELTA_TARGET=-361       # Target shift in mm
#
i=1
fin=$[ $NTHREADS+$i ]
while [ $i -lt $fin ]
do
  cd $MCDIR
  export SEED=$[ $i+$TARGET*1000 ]
  echo "$i "'SEED='$SEED;
  export SUFFIX=${PRODUCTION_NAME}_$i # name of directory where to run production thread.
  mkdir -p $WD/$SUFFIX
  cd $WD/$SUFFIX
  cp -u $MCDIR/generate_1target_qsub.sh $SUFFIX.sh  
  qsub $SUFFIX.sh  #submit the production thread using 'torque' batch system. alice5 is compatible to run 8 threads simultaneously.
  echo $SUFFIX.sh
  let i+=1
done

cd $MCDIR/${PERIOD}_Gener/
