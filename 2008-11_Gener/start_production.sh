#!/bin/bash
#
    export EVENTNUMBER=100000                       # Total event number to be generated per 1 production thread  
    export NTHREADS=128                             # total number of production threads   
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
    export EXTARGET=2
    export HYMC_CONFIG_DEFINED=yes
    export THICKNESS_S4=6 #mm
    export PROBABILITY_SA=0
    
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

#cond=0 # for condition in t2   
#cond=1  # for condition in s4
# for MES in pi0 eta omg f2 2pi0 K0   f0
#for MES in eta_t2 omg_t2 #f2 2pi0 K0 # f0

#========================================================================
#============= Cycle over various conditions ============================
#======================================================================== 
export HYCONDITION
#for HYCONDITION in t2 #s4
for HYCONDITION in t2 s4
do
    case "$HYCONDITION" in
	"t2")    EXTARGET=2 ; cond=1 ;;
	"s4")    EXTARGET=0 ; cond=2 ;;
	"s4")    EXTARGET=3 ; cond=3 ;;
	*)       EXTARGET=0 ; cond=0 ;;
    esac
    #========================================================================
    #============= Cycle over mesons ========================================
    #========================================================================
    # for MES in pi0 eta omg f2 2pi0 K0   f0
    #for MES in eta_t2 omg_t2 #f2 2pi0 K0 # f0
    for MES in rho0 #eta omg
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
	#========================================================================
	#============= Cycle over targets =======================================
	#========================================================================
	#for TGTPRFX in be79mm c78mm al35mm #cu7mm sn5mm pb3mm ch80mm
	for TGTPRFX in ch80mm #cu7mm sn5mm pb3mm
	do
	    UNIC_CODE=$[ $cond*10000000 + $n*1000000 ]
	    export TGT_PRFX=$TGTPRFX
	    export PRODUCTION_NAME=$PERIOD_PRFX$TGT_PRFX  # production name
	    export PRODUCTION_NAME=${PRODUCTION_NAME}_${MESON}
	    if [ -n "$HYCONDITION" ] ; then 
		export PRODUCTION_NAME=${PRODUCTION_NAME}_${HYCONDITION}
	    fi
	    export HYMC_CONFIG_DEFINED=yes
	    export CONTROL=0
	    #	
	    echo
	    echo  MESON =                $MESON
	    echo  TGT_PRFX =             $TGT_PRFX
	    echo 'PERIOD_PRFX =' $PERIOD_PRFX
	    echo  PRODUCTION_NAME =      $PRODUCTION_NAME
	    echo  HYMC_CONFIG_DEFINED =  $HYMC_CONFIG_DEFINED
	    echo 'CONTROL             =' $CONTROL
	    echo 'unic code           =' $UNIC_CODE
	    echo 'EXTARGET            =' $EXTARGET
	    
	    source generate_nov08_be79mm_pi0.sh
	done
    done
done
unset HYMC_CONFIG_DEFINED
