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
# IHEP cluster environment
export MCDIR=/afs/ihep.su/user/s/sevdokim/HyperonMC-factory/  #name of top MC directory
export WD=/lustre/ihep.su/data/hyperon/HYPERON_MC/evdokimov/2008-11_Gener/$PRODUCTION_NAME/
if [ -z $IHEP_QUEUE ] ; then export IHEP_QUEUE=ihep-short ; fi
export SCRATCH="/scratch"

# evdokimov03 environment (local testing)
if [ $(uname -n) = "evdokimov03" ]; then
    MCDIR=~/hyperon/HyperonMC-factory/
    WD=~/hyperon/mc/RegGen/$PRODUCTION_NAME/
    IHEP_QUEUE=thislaptop
    SCRATCH=""
fi
# evdokimov03 environment (local testing) 
if [ $(uname -n) = "alice21" ]; then
    MCDIR=~/hyperon/HyperonMC-factory/
    WD=~/hyperon/mc/RegGen/$PRODUCTION_NAME/
    IHEP_QUEUE=debug
    SCRATCH="/data1/evdokimov/scratch/"
fi


export REGGEN_CARDS=$MCDIR/${PERIOD}_Gener/RegGen_$MESON.cards                  #name of RegGen.cards
export MACRODIR=$MCDIR/Hyp_RegGen/                                              #name of MC tool directory
export MCRUNSDIR=$WD/MCruns/
export ANCHORS=$MCDIR/${PERIOD}_Gener/
export FILELIST=$MCDIR/${PERIOD}_Gener/file_list_$PERIOD_PRFX$TGT_PRFX.dat
#

echo 'queue         =' $IHEP_QUEUE
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
    echo '1target_qsub macro:' $MCDIR/generate_1target_qsub_scratch.sh
    printenv > shell_env.sh
    #use this if in case if some jobs failed and you want to resubmit them
    echo $WD/$SUFFIX && ls -lth $WD/$SUFFIX
    if [ ! -e $MCRUNSDIR/Run${SEED}.gz ] ; then
	if [ -e generated.tar ] || [ -e MC_res.dat ] ; then
	    echo $WD/$SUFFIX && ls -lth $WD/$SUFFIX
	    #qsub -q ihep-short $MCDIR/generate_1target_qsub_scratch.sh
	    #rm -f generated.tar MC_res.dat log* 
	    ln -s $MCDIR/generate_1target_qsub_scratch.sh ${SUFFIX}.sh
	    qsub -q $IHEP_QUEUE ${SUFFIX}.sh
	else
	    #echo $WD/$SUFFIX && ls -lth $WD/$SUFFIX
	    #rm -f generated.tar MC_res.dat log*
	    ln -s $MCDIR/generate_1target_qsub_scratch.sh ${SUFFIX}.sh
	    qsub -q $IHEP_QUEUE ${SUFFIX}.sh
	    #source ${SUFFIX}.sh
	fi
    else
	#ln -s $MCDIR/generate_1target_qsub_scratch.sh ${SUFFIX}.sh
	#qsub -q $IHEP_QUEUE ${SUFFIX}.sh
	echo "${MCRUNSDIR}/Run${SEED}.gz exists"
    fi
    
    # qsub $SUFFIX.sh  #submit the production thread using 'torque' batch system. alice5 is compatible to run 8 threads simultaneously.
    # echo $SUFFIX.sh
    let i+=1
done

cd $MCDIR/${PERIOD}_Gener/
