#!/bin/bash
#
export EVENTNUMBER=100000            # Total event number to be generated per 1 production thread
export NTHREADS=10                   # total number of production threads   
#export IHEP_QUEUE=ihep-medium
export IHEP_QUEUE=ihep-short         # cluster queue name
export UNIC_CODE=0
export USEPYTHIA=1

echo
echo 'EVENTNUMBER =' $EVENTNUMBER		      
echo 'NTHREADS    =' $NTHREADS		    
echo
echo
export START_PRODUCTION_DIR=$(pwd)
for TGTPRFX in be60mm c60mm # s60mm
do
    for en in 0.000
    do
	export PHOTON_ENERGY=$en
	export TGT_PRFX=$TGTPRFX
	export PRODUCTION_NAME=${TGT_PRFX}
	if [ "$USEPYTHIA" = "1" ] ; then PRODUCTION_NAME=${PRODUCTION_NAME}_py8 ; fi
	if [ "$en" != "0.000" ] ; then PRODUCTION_NAME=${PRODUCTION_NAME}_photonE${PHOTON_ENERGY}GeV ; fi
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
  
