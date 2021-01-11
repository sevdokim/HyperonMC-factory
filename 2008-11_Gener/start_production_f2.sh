#!/bin/bash
#
    export EVENTNUMBER=20000                        # Total event number to be generated per 1 production thread  
    export NTHREADS=16                             # total number of production threads   
    export PERIOD=2008-11                          # Hyperon Runs (2007-11, 2008-04, 2008-11, 2009-11, 2011-04 ... -- 15 runs in total) 
    export PERIOD_PRFX=nov08_                      # Period prefix for files:  file_list.dat ==> file_list_nov08.dat  
    export CONVERT_ONLY=no                         # yes or no 
#   export CONVERT_ONLY=yes    
#    
    export TGT_PRFX=pb3mm                          # Hyperon Tgts dlya formirovaniya imeni file_list_nov08.dat ==> file_list_nov08_be79mm.dat
                                                   # TGT_PRFX = be79mm, c78mm, al35mm, al17mm, cu3mm, cu7mm, sn5mm, pb3mm, ch80mm     
    export PRODUCTION_NAME=$PERIOD_PRFX$TGT_PRFX   # production name	 
    export MESON=pi0
    export UNIC_CODE=10000
    export PRODUCTION_NAME=${PRODUCTION_NAME}_$MESON
    export HYMC_CONFIG_DEFINED=yes
    
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
  
n=0
# for MES in pi0 eta omg f2 2pi0 K0 f2Mike
# for MES in pi0 eta omg f2 2pi0 # K0 f2Mike
  for MES in f2  #  pi0 eta omg f2 2pi0 K0 f2Mike
do 
    j=0
    let n+=1
    export MESON=$MES
    for TGTPRFX in be79mm c78mm # al35mm cu7mm sn5mm pb3mm
    do
	let j+=1

	UNIC_CODE=$[ $n*1000000+ $j*100000 ]
	export TGT_PRFX=$TGTPRFX
	export PRODUCTION_NAME=$PERIOD_PRFX$TGT_PRFX  # production name
	export PRODUCTION_NAME=${PRODUCTION_NAME}_$MESON
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
	
	source generate_nov08_be79mm_pi0.sh
    done
done
unset HYMC_CONFIG_DEFINED
