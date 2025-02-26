if [ $TSIM_CONFIG_DEFINED = yes ]
then
    unset TSIM_CONFIG_DEFINED
    echo 'Using predefined config!'
else
    echo 'Using default config!'
    export EVENTNUMBER=1000    # Total event number to be generated per 1 production thread
    export NTHREADS=1          # total number of production threads
    export UNIC_CODE=10000
    export USEPYTHIA=1
    export TGT_PRFX=be60mm
    export PHOTON_ENERGY=0.0044
    export PRODUCTION_NAME=${TGT_PRFX}_py8_photonE${PHOTON_ENERGY}GeV
fi
#cluster environment
export MCDIR=/afs/ihep.su/user/s/sevdokim/HyperonMC-factory/targetSim/source  #name of top MC directory
export WD=/lustre/ihep.su/data/hyperon/HYPERON_MC/evdokimov/targetSim/$PRODUCTION_NAME/
export scratch=/scratch
#evdokimov03 environment
if [ $(uname -n) = "evdokimov03" ] ; then
    MCDIR=/home/evdokimov/hyperon/HyperonMC-factory/targetSim/source
    WD=/home/evdokimov/hyperon/mc/targetSim/$PRODUCTION_NAME/
    scratch=""
    IHEP_QUEUE=thislaptop
fi
if [ $(uname -n) = "alice21" ] ; then
    MCDIR=/home/evdokimov/hyperon/HyperonMC-factory/targetSim/source
    WD=/home/evdokimov/hyperon/mc/targetSim/$PRODUCTION_NAME/
    scratch=""
    IHEP_QUEUE=debug
fi

export MACRODIR=$MCDIR                         #name of MC tool directory
#
if [ -z $IHEP_QUEUE ] ; then export IHEP_QUEUE=debug ; fi
echo 'queue         =' $IHEP_QUEUE
echo
echo 'MCDIR         =' $MCDIR
echo 'MACRODIR      =' $MACRODIR
echo 'WD            =' $WD
echo
#
case "$TGT_PRFX" in 
    "be60mm")   export TARGET=1; export TTHICKNESS=6; export TRADIUS=2 ;;
    "c60mm")    export TARGET=2; export TTHICKNESS=6; export TRADIUS=2 ;;
    *)          echo "$TGT_PRFX is incorrect. Stopping"; return ;; 
esac
#
# export TARGET=2            # Tgts material for GEANT (1=Be, 2=C) 
echo 'Medium of the main target               = ' $TARGET
echo '(1=Be, 2=C)' 
echo
#

i=1
fin=$[ $NTHREADS+$i ]
while [ $i -lt $fin ]
do
    cd $MCDIR
    export SEED=$[ $i+$TARGET*10000+$UNIC_CODE ]
    echo "Thread $i, SEED=$SEED"
    export SUFFIX=${PRODUCTION_NAME}_$i # name of directory where to run production thread.
    #
    mkdir -p $WD/$SUFFIX
    cd $WD/$SUFFIX 
    # if [ ! -f Run${SEED}.gz ] ; then # this is attempt to resubmit failed jobs. 
    echo '1target_qsub macro:' $MCDIR/generate_1thread.sh
    printenv > shell_env.sh
    #use this if in case if some jobs failed and you want to resubmit them
    echo $WD/$SUFFIX && ls -lth $WD/$SUFFIX
	ln -s $MCDIR/generate_1thread.sh ${SUFFIX}.sh
    echo "submitting job ${SUFFIX}"
	qsub -q $IHEP_QUEUE ${SUFFIX}.sh
    let i+=1
done

cd $START_PRODUCTION_DIR
