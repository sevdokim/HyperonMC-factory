#!/bin/bash
#
export EVENTNUMBER=100000            # Total event number to be generated per 1 production thread
export NTHREADS=10                   # total number of production threads   
#export IHEP_QUEUE=ihep-medium
export IHEP_QUEUE=ihep-short         # cluster queue name
export UNIC_CODE=0
export USEPYTHIA=1
export PHOTON_ENERGY

echo
echo 'EVENTNUMBER =' $EVENTNUMBER		      
echo 'NTHREADS    =' $NTHREADS		    
echo
echo
export START_PRODUCTION_DIR=$(pwd)
for TGTPRFX in c60mm # s60mm be60mm
do
    for PHOTON_ENERGY in 0.0023
    do
	export TGT_PRFX=$TGTPRFX
	export PRODUCTION_NAME=${TGT_PRFX}
	if [ "$USEPYTHIA" = "1" ] ; then PRODUCTION_NAME=${PRODUCTION_NAME}_py8 ; fi
	PRODUCTION_NAME=${PRODUCTION_NAME}_photonE${PHOTON_ENERGY}GeV_GNTsingleRawCsI
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
  
