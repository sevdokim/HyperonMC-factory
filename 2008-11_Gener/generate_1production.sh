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
    export EVENTNUMBER=10                     # Total event number to be generated per 1 production thread
    export NTHREADS=1                             # total number of production threads
    export PERIOD=2008-11                         # Hyperon Runs (2007-11, 2008-04, 2008-11, 2009-11, 2011-04 ... -- 15 runs in total) 
    export PERIOD_PRFX=nov08_                     # Period prefix for files:  file_list.dat ==> file_list_nov08.dat
    export TGT_PRFX=be79mm                         # Hyperon Tgts dlya formirovaniya imeni file_list_nov08.dat ==> file_list_nov08_be79mm.dat
                                                  # TGT_PRFX = be79mm, c78mm, al35mm, al17mm, cu3mm, cu7mm, sn5mm, pb=3mm, ch80mm
    export PRODUCTION_NAME=$PERIOD_PRFX$TGT_PRFX  # production name
    export MESON=pi0
    export UNIC_CODE=10000
    export PRODUCTION_NAME=${PRODUCTION_NAME}_$MESON
    export CONVERT_ONLY=yes                       # yes or no
    export EXTARGET=0                             #exclusive target (1=S4, 2=target, 3=Sa, 0=all of them)
fi
#
#default values, can be redefined via config/config.$THIS_COMPUTER_NAME file
export MCDIR=/lustre/ihep.su/data/hyperon/HYPERON_MC/evdokimov/  #name of top MC directory where to run production
export MACRODIR=/afs/ihep.su/user/s/sevdokim/HyperonMC-tools/ #name of directory  
if [ ! -z $HYPERONMC_FACTORY ] ; then 
    
fi
export REGGEN_CARDS=$MCDIR/${PERIOD}_Gener/RegGen_$MESON.cards                  #name of RegGen.cards
export MACRODIR=/afs/ihep.su/user/s/sevdokim/HyperonMC-tools                                              #name of MC tool directory
#export ANDIR=/data1/users/sadovsky/hyperon/Hyperon_2007_2018_Rev/               #name of top analysis directory (not used)
export WD=$MCDIR/${PERIOD}_Gener/$PRODUCTION_NAME/  # name of this production dir
export MCRUNSDIR=$MCDIR/${PERIOD}_Gener/$PRODUCTION_NAME/MCruns/
#export WD=/lustre/ihep.su/data/hyperon/HYPERON_MC/evdokimov/${PERIOD}_Gener/$PRODUCTION_NAME
#export MCRUNSDIR=$WD/MCruns
export ANCHORS=$MCDIR/${PERIOD}_Gener/
export FILELIST=$MCDIR/${PERIOD}_Gener/file_list_$PERIOD_PRFX$TGT_PRFX.dat
#export FILELIST=$MCDIR/${PERIOD}_Gener/file_lists/file_list_$PERIOD_PRFX$TGT_PRFX.dat
#
echo
echo 'MCDIR         =' $MCDIR
echo 'MACRODIR      =' $MACRODIR
echo 'ANCHORS       =' $ANCHORS
echo 'REGGEN_CARDS  =' $REGGEN_CARDS
echo 'FILELIST      =' $FILELIST
echo 'ANDIR         =' $ANDIR
echo 'WD            =' $WD
echo 'MCRUNSDIR     =' $MCRUNSDIR
echo
#
mkdir -p $MCRUNSDIR
#
case "$TGT_PRFX" in 
    "be79mm")	export TARGET=2	;;
    "c78mm")	export TARGET=10;;
    "al35mm")	export TARGET=3	;;
    "al17mm")	export TARGET=3	;;
    "cu3mm")	export TARGET=9	;;
    "cu7mm")	export TARGET=9	;;
    "sn5mm")	export TARGET=8	;;
    "pb3mm")    export TARGET=7 ;;
    "ch80mm")   export TARGET=11;;
    *)          export TARGET=2 ;; #default value (Be)
esac
#
# export TARGET=2            # Tgts material for GEANT (2=Be, 10=C, 3=Al, 9=Cu, 8=Sn, 7=Pb, 11=polyethylen) 
echo 'Target type (1=S1, 2=Targ, 3=Sa, 0=All) = ' $EXTARGET
echo 'Medium of the main target               = ' $TARGET
echo '(2=Be, 10=C, 3=Al, 9=Cu, 8=Sn, 7=Pb, 11=polyethylen)' 
echo
#

#i=$UNIC_CODE
i=0
fin=$[ $NTHREADS+$i ]
while [ $i -lt $fin ]
do
  cd $MCDIR
  export SEED=$[ $i+$TARGET*10000+$UNIC_CODE ]
  echo "$i "'SEED='$SEED;
  export SUFFIX=${PRODUCTION_NAME}_$i # name of directory where to run production thread.
#
  mkdir -p $WD/$SUFFIX
  cd $WD/$SUFFIX 
 # if [ ! -f Run${SEED}.gz ] ; then # this is attempt to resubmit failed jobs. 
  echo '1target_qsub macro:' $MCDIR/generate_1target_qsub.sh
  printenv > shell_env.sh
  qsub -q ihep-short $MCDIR/generate_1target_qsub.sh
 # fi
# qsub $SUFFIX.sh  #submit the production thread using 'torque' batch system. alice5 is compatible to run 8 threads simultaneously.
# echo $SUFFIX.sh
  let i+=1
done

cd $MCDIR/${PERIOD}_Gener/
