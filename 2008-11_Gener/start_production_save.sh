    export EVENTNUMBER=100000                      # Total event number to be generated per 1 production thread  
    export NTHREADS=16                             # total number of production threads   
    export PERIOD=2008-11                         # Hyperon Runs (2007-11, 2008-04, 2008-11, 2009-11, 2011-04 ... -- 15 runs in total) 
    export PERIOD_PRFX=nov08_                     # Period prefix for files:  file_list.dat ==> file_list_nov08.dat  
    export TGT_PRFX=pb3mm                        # Hyperon Tgts dlya formirovaniya imeni file_list_nov08.dat ==> file_list_nov08_be79mm.dat
                                                  # TGT_PRFX = be79mm, c78mm, al35mm, al17mm, cu3mm, cu7mm, sn5mm, pb3mm, ch80mm     
    export PRODUCTION_NAME=$PERIOD_PRFX$TGT_PRFX  # production name     
    export MESON=pi0
    export UNIC_CODE=10000
    export PRODUCTION_NAME=${PRODUCTION_NAME}_$MESON
    export CONVERT_ONLY=yes                        # yes or no 
    export HYMC_CONFIG_DEFINED=yes
n=0
for MES in f2Mike 2pi0 pi0 eta K0 f2 omg 
do 
    j=0
    let n+=1
    export MESON=$MES
    for TGTPRFX in be79mm c78mm # al35mm cu7mm sn5mm pb3mm
    do
	let j+=1
	UNIC_CODE=$[ $n*1000000+ $j*100000 ]
	echo 'unic code = ' $UNIC_CODE
	export TGT_PRFX=$TGTPRFX
	export PRODUCTION_NAME=$PERIOD_PRFX$TGT_PRFX  # production name
	export PRODUCTION_NAME=${PRODUCTION_NAME}_$MESON
	export HYMC_CONFIG_DEFINED=yes
	export CONTROL=0
	source generate_nov08_be79mm_pi0.sh
    done
done
unset HYMC_CONFIG_DEFINED