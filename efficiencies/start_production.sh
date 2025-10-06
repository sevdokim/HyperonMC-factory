#!/bin/bash
#
export IHEP_QUEUE=ihep-medium                  # cluster queue name
export EVENTNUMBER=100000                     # Total event number to be generated per 1 production thread  
export NTHREADS=10                             # total number of production threads   
export CONVERT_ONLY=no                         # just reconvert generated data to Hyperon format

export PERIOD                          # Hyperon Runs (2007-11, 2008-04, 2008-11, 2009-11, 2011-04 ... -- 15 runs in total) 
export PERIOD_PRFX                     # Period prefix for files:  file_list.dat ==> file_list_nov08.dat  
export TGT_PRFX
export MESON
export EXTARGET
INITIAL_DIR=$(pwd)

#for HYCONDITION in s4
for HYCONDITION in t2
do
    case "$HYCONDITION" in
        "t2")    EXTARGET=2 ; cond=1 ;;
        "s4")    EXTARGET=1 ; cond=2 ;;
	"sa")    EXTARGET=3 ; cond=3 ;;
        *)       EXTARGET=0 ; cond=0 ;;
    esac
    for PERIOD_PRFX in nov15 apr12 apr13 mar15 nov12 #apr18
    do
	PERIOD=$(period_by_prefix $PERIOD_PRFX)
	case "$PERIOD_PRFX" in 
	    apr12)  export EFFICIENCY_SA=1.0 ;;
	    apr13)  export EFFICIENCY_SA=0.986 ;;
	    mar15)  export EFFICIENCY_SA=1.0 ;;
	    nov12)  export EFFICIENCY_SA=0.98 ;;
	    nov15)  export EFFICIENCY_SA=0.991 ;;
	    *)      export EFFICIENCY_SA=1.0 ;;
	esac
	export EFFICIENCY_SA=1.0 # 100 percent efficiency
	for MESON in pi0 eta omg K0 f2 #2pi0 K0   f0
	#for MES in eta #f2 #omg #eta f2 2pi0 K0 # f0
	do 	
	    case "$MES" in
		"pi0")     n=1 ;;
		"eta")     n=2 ;;
		"omg")     n=3 ;;
		"K0")      n=4 ;;
		"f2")      n=5 ;;
		*)         n=0 ;;
	    esac
	    for TGT_PRFX in al35mm cu7mm sn5mm pb3mm c78mm be79mm #ch80mm # al35mm cu7mm sn5mm pb3mm
	    #for TGT_PRFX in be79mm c78mm
	    do
		cd $INITIAL_DIR
		if [ -e $PERIOD/file_list_${PERIOD_PRFX}_${TGT_PRFX}.dat ] ; then
		    export THICKNESS_S4=1.0 # from 2009
		    # export THICKNESS_S4=6.0 # before 2009
		    export UNIC_CODE=$[ $cond*1000000+ $n*100000 ]
		    export PRODUCTION_NAME=${PERIOD_PRFX}_${TGT_PRFX}_${MESON}_PDG_${HYCONDITION}_effSa1.0
		    export HYMC_CONFIG_DEFINED=yes
		    
		    echo " "
		    echo 'MESON               =' $MESON
		    echo 'TGT_PRFX            =' $TGT_PRFX
		    echo 'PERIOD_PRFX         =' $PERIOD_PRFX
		    echo 'PRODUCTION_NAME     =' $PRODUCTION_NAME
		    echo 'HYMC_CONFIG_DEFINED =' $HYMC_CONFIG_DEFINED
		    echo 'CONTROL             =' $CONTROL
		    echo 'unic code           =' $UNIC_CODE
		    source generate_production.sh
		    cd $INITIAL_DIR
		else
		    echo "$PERIOD/file_list_${PERIOD_PRFX}_${TGT_PRFX}.dat does not exists. Skipping."
		fi
	    done
	done
    done
done
unset HYMC_CONFIG_DEFINED
