#!/bin/bash
#
    export EVENTNUMBER=1000000                     # Total event number to be generated per 1 production thread  
    export NTHREADS=1                              # total number of production threads   
    export PERIOD=2008-11                          # Hyperon Runs (2007-11, 2008-04, 2008-11, 2009-11, 2011-04 ... -- 15 runs in total) 
    export PERIOD_PRFX=nov08_                      # Period prefix for files:  file_list.dat ==> file_list_nov08.dat  
#   export CONVERT_ONLY=yes                        # yes or no 
    export CONVERT_ONLY=no    
#    
    export TGT_PRFX=pb3mm                          # Hyperon Tgts dlya formirovaniya imeni file_list_nov08.dat ==> file_list_nov08_be79mm.dat
                                                   # TGT_PRFX = be79mm, c78mm, al35mm, al17mm, cu3mm, cu7mm, sn5mm, pb3mm, ch80mm     
    export PRODUCTION_NAME=$PERIOD_PRFX$TGT_PRFX   # production name     
    export MESON=pi0
    export UNIC_CODE=10000
    export PRODUCTION_NAME=${PRODUCTION_NAME}_$MESON
    export THICKNESS_S4=6 #mm
    export PROBABILITY_SA=0
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
  
export HYCONDITION
#for HYCONDITION in s4
for HYCONDITION in sa #t2 s4
do
    case "$HYCONDITION" in
        "t2")    EXTARGET=2 ; cond=1 ;;
        "s4")    EXTARGET=0 ; cond=2 ;;
	"sa")    EXTARGET=3 ; cond=3 ;;
        *)       EXTARGET=0 ; cond=0 ;;
    esac
    # for MES in pi0 eta omg f2 2pi0 K0   f0
    for MES in omg #eta f2 2pi0 K0 # f0
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
	for TGTPRFX in al35mm cu7mm sn5mm pb3mm c78mm be79mm # al35mm cu7mm sn5mm pb3mm
#	for TGTPRFX in be79mm
	do
	    for width in {5..16} #c is control parameter for omg width 
	    do
		MESON=${MES}_${HYCONDITION}_width${width}MeV
		UNIC_CODE=$[ $cond*1000000+ $n*100000 + $width*1000 ]
		export TGT_PRFX=$TGTPRFX
		export PRODUCTION_NAME=$PERIOD_PRFX$TGT_PRFX  # production name
		export PRODUCTION_NAME=${PRODUCTION_NAME}_${MESON}
		export HYMC_CONFIG_DEFINED=yes
		export CONTROL=$width	
		echo " "
		echo  MESON =                $MESON
		echo  TGT_PRFX =             $TGT_PRFX
		echo 'PERIOD_PRFX =' $PERIOD_PRFX
		echo  PRODUCTION_NAME =      $PRODUCTION_NAME
		echo  HYMC_CONFIG_DEFINED =  $HYMC_CONFIG_DEFINED
		echo 'CONTROL             =' $CONTROL
		echo 'unic code           =' $UNIC_CODE
		source generate_nov08_be79mm_pi0.sh
	    done
	done
    done
done
  unset HYMC_CONFIG_DEFINED
