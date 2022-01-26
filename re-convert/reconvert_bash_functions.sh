function reconvert_threads () {
    if [ -z $HYCONVERTER]; then 
	HYCONVERTER=$REPO_HYPERONMC_FACTORY/converter/exe
    else
	HYCONVERTER=$REPO_HYPERONMC_FACTORY/Hyp_RegGen/converter/exe
    fi
    if ! /bin/ls $HYCONVERTER ; then echo "cannot find converter! stopping"; return 1; fi
    
    this_wd=$(pwd)
    if [ -z $1 ] || [ -z $2 ] || [ -z $3 ]; then
	echo "Usage: reconvert_threads /path/to/threads/list /path/to/anchors period1:period2:...:periodN "
    fi
    if [ -z $RECONVERTING_TOP_DIRECTORY ] || [ -z $production_name ] ; then
	echo "Please export RECONVERTING_TOP_DIRECTORY=..."
	echo "Please export production_name=..."
    fi
    if [ ! -f $1 ] ; then
	echo "no list of production threads found: $1"
	return 1 
    fi

    target=$(echo $production_name | awk -F'_' '{ print $2}')
    echo "I recieved folloving period list: $3"
    for period in $(echo $3 | awk -F':' '{ for ( i=1; i<=NF; i++) print $i}')
    do
	if /bin/ls $2/$period ; then
	    echo "I found $2/$period, linking anchor files to $this_wd/$period"
	    mkdir -p /scratch/$USER/$this_wd/$period/
	    mkdir -p $RECONVERTING_TOP_DIRECTORY/$production_name/$period/MCruns
	    for anchor in calibr.cards coeff_old.dat e_cor_matrix.dat h_s_new.dat bad_channels.dat ; do 
		cp $2/$period/$anchor /scratch/$USER/$this_wd/$period/$anchor
	    done
	    if [ -f $2/$period/file_list_$(prefix_by_period $period)_${target}.dat ] ; then
		echo "I found file_list.dat for period $period :"
		echo "$2/$period/file_list_$(prefix_by_period $period)_${target}.dat"
		cp $2/$period/file_list_$(prefix_by_period $period)_${target}.dat /scratch/$USER/$this_wd/$period/file_list.dat
	    fi
	fi
    done
    for thread in $(cat $1) ; do
	if cd $thread ; then
	    if [ -f log_production_part ] ; then
		SEED="$(grep 'seed =' log_production_part | awk -F' ' '{print $NF}')"
		echo "SEED=$SEED"
	    else 
		echo "cannot get seed from log_production_part: file does not exist"
	    fi
	    if [ -f generated.tar ] ; then
		tar -xf generated.tar MC_res.dat.bz2
		bzip2 -d MC_res.dat.bz2
		if ls MC_res.dat ; then 
		    echo "Unpacked file $(pwd)/MC_res.dat" 
		    if mv MC_res.dat /scratch/$USER/$this_wd && cd /scratch/$USER/$this_wd ; then
			echo "moved MC_res.dat to /scratch/$USER/$this_wd and cd-ed there."
			for period in $(echo $3 | awk -F':' '{ for ( i=1; i<=NF; i++) print $i}')
			do
			    echo "I go to period $period"
			    cd /scratch/$USER/$this_wd/$period
			    if [ -f file_list.dat ] ; then
				$HYCONVERTER ../MC_res.dat ${SEED} >& log_converter_${SEED}
				mv Run${SEED}.gz $RECONVERTING_TOP_DIRECTORY/$production_name/$period/MCruns
				mv log_converter_${SEED} $RECONVERTING_TOP_DIRECTORY/$production_name/$period/reconvert_thread_$i
			    else
				echo "no file_list.dat for $period in /scratch/$USER/$this_wd/$period"
			    fi
			done
			cd /scratch/$USER/$this_wd
			rm MC_res.dat
		    else
			echo "problem occured when being moving MC_res.dat to /scratch/$USER/$this_wd and cd-ing there."
		    fi
		else echo "Problem occured while being unpacking $(pwd)/MC_res.dat" ; fi
	    else
		echo "No generated.tar file found. Probably something wrong with production $thread."
	    fi
	else 
	    echo "cannot find production thread $thread";
	fi
	cd $this_wd
    done
    cd $this_wd
}
export -f reconvert_threads
function reconvert_production () {
    if [ -z $1 ] || [ -z $2 ] || [ -z $3 ] ; then
	echo "Usage: reconvert_production /path/to/production /path/to/anchors period1:period2:...:periodN Nthreads"
	echo "where /path/to/production is path to production to reconvert"
	echo "/path/to/anchors/ is path to dir with anchors to be used by converter"
	echo "period1:period2:...:periodN is list of periods to be used for reconversion"
	echo "Nthreads is number of production threads to reconvert per one job"
	return 1
    fi
    if [ -z $RECONVERTING_TOP_DIRECTORY ] || ! mkdir -p $RECONVERTING_TOP_DIRECTORY; then
	echo "Please export RECONVERTING_TOP_DIRECTORY=/absolute/path/to/top/reconvert/directory"
	return 1
    fi
    this_wd=$(pwd)
    export production_name=$(echo $1 | awk -F'/' '{print $NF}')
    production_prefix=$(echo $production_name | awk -F'_' '{print $1}' )
    production_period=$(period_by_prefix $production_prefix)
    /bin/ls -d -1 $1/${production_prefix}* > thread_list
    if [ -z $4 ] ; then max_threads=0 ; else max_threads=$4 ; fi
    echo "max number of production threads per job = $max_threads"
    line_count=0
    submit_number=0
    rm -rf thread_list_*
    while read line ; do 
	let line_count+=1 
	echo $line >> thread_list_$submit_number
	if [ $line_count -eq $max_threads ] ; then
	    line_count=0
	    let submit_number+=1
	fi
    done < thread_list
    mkdir -p $RECONVERTING_TOP_DIRECTORY/$production_name
    mv thread_list* $RECONVERTING_TOP_DIRECTORY/$production_name
    for (( i=0 ; i<=submit_number ; i++ )) ; do
	cd $RECONVERTING_TOP_DIRECTORY/$production_name
	if [ -f thread_list_$i ] ; then
	    echo "submit reconversion ${i}. Threads list:"
            cat thread_list_$i
	    mkdir -p $RECONVERTING_TOP_DIRECTORY/$production_name/reconvert_thread_$i
	    mv thread_list_$i $RECONVERTING_TOP_DIRECTORY/$production_name/reconvert_thread_$i
	    cd $RECONVERTING_TOP_DIRECTORY/$production_name/reconvert_thread_$i
	    echo "thread_list_$i $2 $3 $max_threads" > reconvert_threads.parameters
	    if [ $USE_TORQUE = YES ] ; then qsub $REPO_HYPERONMC_FACTORY/re-convert/reconvert_1thread.sh ;
	    else reconvert_threads thread_list_$i $2 $3 $max_threads ; fi
	fi
    done
    cd $this_wd
}
export -f reconvert_production
