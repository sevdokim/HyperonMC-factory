#!/bin/bash
#
    export EVENTNUMBER=200000            # Total event number to be generated per 1 production thread
    export NTHREADS=50                   # total number of production threads   
    #export IHEP_QUEUE=ihep-medium
    export IHEP_QUEUE=ihep-short         # cluster queue name
    export PERIOD=2008-11                # U70 runs (2007-11, 2008-04, ... -- 15 runs in total) 
#    export CONVERT_ONLY=yes             # yes or no 
    export CONVERT_ONLY=no    
#    
    export TGT_PRFX=pb3mm                # TGT_PRFX = be79mm, c78mm, al35mm, al17mm, cu3mm, cu7mm, sn5mm, pb3mm, ch80mm     
    export PRODUCTION_NAME=$(prefix_by_period $PERIOD)_$TGT_PRFX   # production name     
    export MESON=incl
    export UNIC_CODE=10000
    export PRODUCTION_NAME=${PRODUCTION_NAME}_$MESON
    export THICKNESS_S4=1 #mm
    export HYMC_CONFIG_DEFINED=yes
    export EXTARGET

    echo
    echo 'EVENTNUMBER =' $EVENTNUMBER		      
    echo 'NTHREADS    =' $NTHREADS		    
    echo 'PERIOD      =' $PERIOD  		  
    echo 'PERIOD_PRFX =' $PERIOD_PRFX		     
    echo 'CONVERT_ONLY=' $CONVERT_ONLY	
    echo
    echo 'MESON (dummy)               =' $MESON
    echo 'TGT_PRFX (dummy)            =' $TGT_PRFX
    echo 'PRODUCTION_NAME (dummy)     =' $PRODUCTION_NAME
    echo 'HYMC_CONFIG_DEFINED (dummy) =' $HYMC_CONFIG_DEFINED 
    echo 'UNIC_CODE (dummy)           =' $UNIC_CODE	    
    echo
export START_PRODUCTION_DIR=$(pwd)
export HYCONDITION
#for HYCONDITION in s4
for HYCONDITION in s4t2sa empty
do
    unset PROBABILITY_TARG
    case "$HYCONDITION" in
        "t2")     EXTARGET=2 ; cond=1 ;; # interactions with target only
        "s4t2sa") EXTARGET=0 ; cond=0 ;; # interactions with s4, target and sa
	"sa")     EXTARGET=3 ; cond=3 ;; # interactions with sa
	"empty")  EXTARGET=0 ; cond=4 ; export PROBABILITY_TARG=0 ;; #empty target -> interactions with s4 and sa
        *)        EXTARGET=0 ; cond=0 ;;
    esac
    # for MES in pi0 eta omg f2 2pi0 K0   f0
    for MES in incl #f2 #omg #eta f2 2pi0 K0 # f0
    do 	
	case "$MES" in
            "pi0")     n=1 ;;
            "eta")     n=2 ;;
            "omg")     n=3 ;;
            "K0")      n=4 ;;
            "f2")      n=5 ;;
            "2pi0")    n=6 ;;
            "f0")      n=7 ;;
            "rho0")    n=8 ;;
            *)         n=0 ;;
	esac
	export MESON=$MES
#	for TGTPRFX in al35mm cu7mm sn5mm pb3mm c78mm be79mm ch80mm # al35mm cu7mm sn5mm pb3mm
	for TGTPRFX in pb3mm al35mm cu7mm sn5mm c78mm be79mm
	do
	    for per in 2012-04 2012-11 2015-03 2014-11 2013-03
	    do
		unset DELTA_SA
		if [ $per = 2015-03 ] ; then 
		    export DELTA_SA=110. # shift Sa to LGD2 by 11 cm
		fi
		export PERIOD=$per
		MESON=${MES}
		UNIC_CODE=$[ $cond*1000000+ $n*100000 ]
		export TGT_PRFX=$TGTPRFX
		export PRODUCTION_NAME=${TGT_PRFX}_${MES}_${HYCONDITION}
		export HYMC_CONFIG_DEFINED=yes
		echo " "
		echo  MESON =                $MESON
		echo  TGT_PRFX =             $TGT_PRFX
		echo 'PERIOD_PRFX =' $(prefix_by_period $per)
		echo  PRODUCTION_NAME =      $PRODUCTION_NAME
		echo  HYMC_CONFIG_DEFINED =  $HYMC_CONFIG_DEFINED
		echo 'unic code           =' $UNIC_CODE
		source generate_py8.sh
	    done
	done
    done
done
unset HYMC_CONFIG_DEFINED
  
