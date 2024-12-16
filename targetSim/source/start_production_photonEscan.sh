#!/bin/bash
#
export EVENTNUMBER=100000            # Total event number to be generated per 1 production thread
export NTHREADS=10                   # total number of production threads   
#export IHEP_QUEUE=ihep-medium
export IHEP_QUEUE=ihep-short         # cluster queue name
export UNIC_CODE=0
export USEPYTHIA=0

echo
echo 'EVENTNUMBER =' $EVENTNUMBER		      
echo 'NTHREADS    =' $NTHREADS		    
echo
echo
export START_PRODUCTION_DIR=$(pwd)
for TGTPRFX in be60mm c60mm # s60mm
do
    for en in 0.000 0.001 0.002 0.003 0.004 0.005 0.006 0.007 0.008 0.009 0.010 0.015 0.020 0.025 0.035
    do
	export PHOTON_ENERGY=$en
	export TGT_PRFX=$TGTPRFX
	export PRODUCTION_NAME=${TGT_PRFX}
	if [ "$USEPYTHIA" = "1" ] ; then PRODUCTION_NAME=${PRODUCTION_NAME}_py8 ; fi
	PRODUCTION_NAME=${PRODUCTION_NAME}_photonE${PHOTON_ENERGY}GeV
	export TSIM_CONFIG_DEFINED=yes
	echo " "
	echo  TGT_PRFX =             $TGT_PRFX
	echo  PRODUCTION_NAME =      $PRODUCTION_NAME
	echo  TSIM_CONFIG_DEFINED =  $TSIM_CONFIG_DEFINED
	echo 'unic code           =' $UNIC_CODE
	source generate_production.sh
    done
done
unset TSIM_CONFIG_DEFINED
  
