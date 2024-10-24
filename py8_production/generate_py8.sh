if [ $HYMC_CONFIG_DEFINED = yes ]
then
    unset HYMC_CONFIG_DEFINED
    echo 'Using predefined config!'
else
    export EVENTNUMBER=1000    # Total event number to be generated per 1 production thread
    export NTHREADS=1          # total number of production threads
    export PERIOD=2008-11      # U70 session period (2007-11, 2008-04,...  -- 15 runs in total) 
    export TGT_PRFX=be79mm     # Hyperon targe
    export MESON=incl                              
    export UNIC_CODE=10000
    export CONVERT_ONLY=yes    # yes or no
    export EXTARGET=0          #exclusive target (1=S4, 2=target, 3=Sa, 0=all of them)
    export PRODUCTION_NAME=${TGT_PRFX}_${MESON}
fi
#
export MCDIR=/afs/ihep.su/user/s/sevdokim/HyperonMC-factory/  #name of top MC directory
export MACRODIR=$MCDIR/Hyp_py8/source                         #name of MC tool directory
export WD=/lustre/ihep.su/data/hyperon/HYPERON_MC/evdokimov/py8_production/$PERIOD/$PRODUCTION_NAME/
export MCRUNSDIR=$WD/MCruns/
export ANCHORS=$MCDIR/py8_production/${PERIOD}/
export FILELIST=$ANCHORS/file_list_$(prefix_by_period $PERIOD)_$TGT_PRFX.dat
#
if [ -z $IHEP_QUEUE ] ; then export IHEP_QUEUE=ihep-short ; fi
echo 'queue         =' $IHEP_QUEUE
echo
echo 'MCDIR         =' $MCDIR
echo 'MACRODIR      =' $MACRODIR
echo 'ANCHORS       =' $ANCHORS
echo 'FILELIST      =' $FILELIST
echo 'WD            =' $WD
echo 'MCRUNSDIR     =' $MCRUNSDIR
echo
#
if [ ! -e $FILELIST ] ; then
    echo "$FILELIST does not exist. Skipping the production..."
    return
fi
#
mkdir -p $MCRUNSDIR
#
case "$TGT_PRFX" in 
    "be79mm")	export TARGET=2	;;
    "be60mm")   export TARGET=2 ;;
    "c78mm")	export TARGET=10;;
    "c60mm")    export TARGET=10;;
    "al35mm")	export TARGET=3	;;
    "al17mm")	export TARGET=3	;;
    "cu3mm")	export TARGET=9	;;
    "cu7mm")	export TARGET=9	;;
    "sn5mm")	export TARGET=8	;;
    "pb3mm")    export TARGET=7 ;;
    "ch80mm")   export TARGET=11;;
    "s60mm")    export TARGET=12;;
    *)          echo "$TGT_PRFX is incorrect. Stopping"; return ;; 
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
    echo '1target_qsub macro:' $MCDIR/generate_1target_py8.sh
    printenv > shell_env.sh
    #use this if in case if some jobs failed and you want to resubmit them
    echo $WD/$SUFFIX && ls -lth $WD/$SUFFIX
    if [ ! -e $MCRUNSDIR/Run${SEED}.gz ] ; then
	if [ -e generated.tar ] || [ -e MC_res.dat ] ; then
	    echo $WD/$SUFFIX && ls -lth $WD/$SUFFIX
	    #qsub -q ihep-short $MCDIR/generate_1target_qsub_scratch.sh
	    #rm -f generated.tar MC_res.dat log* 
	    ln -s $MCDIR/generate_1target_py8.sh ${SUFFIX}.sh
	    qsub -q $IHEP_QUEUE ${SUFFIX}.sh
	else
	    #echo $WD/$SUFFIX && ls -lth $WD/$SUFFIX
	    #rm -f generated.tar MC_res.dat log*
	    ln -s $MCDIR/generate_1target_py8.sh ${SUFFIX}.sh
	    qsub -q $IHEP_QUEUE ${SUFFIX}.sh
	    #source ${SUFFIX}.sh
	fi
    else
	#ln -s $MCDIR/generate_1target_py8.sh ${SUFFIX}.sh
	#qsub -q $IHEP_QUEUE ${SUFFIX}.sh
	echo "${MCRUNSDIR}/Run${SEED}.gz exists"
    fi
    
    # qsub $SUFFIX.sh  #submit the production thread using 'torque' batch system. alice5 is compatible to run 8 threads simultaneously.
    # echo $SUFFIX.sh
    let i+=1
done

cd $START_PRODUCTION_DIR
