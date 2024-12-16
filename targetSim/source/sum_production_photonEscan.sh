#!/bin/bash
#
USEPYTHIA=1
for TGTPRFX in be60mm c60mm # s60mm
do
    for PHOTON_ENERGY in 0.000 0.001 0.002 0.003 0.004 0.0044 0.005 0.006 0.007 0.008 0.009 0.010 0.015 0.020 0.025 0.035
    do
	PRODUCTION_NAME=${TGTPRFX}
	if [ "$USEPYTHIA" = "1" ] ; then PRODUCTION_NAME=${PRODUCTION_NAME}_py8 ; fi
	PRODUCTION_NAME=${PRODUCTION_NAME}_photonE${PHOTON_ENERGY}GeV
	ls ~/hyperon/mc/targetSim/$PRODUCTION_NAME/histos/* -d > ${PRODUCTION_NAME}.list
	./addhisto.sh ${PRODUCTION_NAME}.list ${PRODUCTION_NAME}.root
    done
done

  
