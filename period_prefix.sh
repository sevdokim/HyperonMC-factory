function prefix_by_period () {
    #======= first, read period list ================
    if [ -z ${hy_periods_names} ]; then
	if [ -z $REPO_HYPERONMC_FACTORY ]; then
	    REPO_HYPERONMC_FACTORY=$(dirname $(realpath "${BASH_SOURCE[0]}"))
	    echo "No REPO_HYPERONMC_FACTORY setted. I set it to be ${REPO_HYPERONMC_FACTORY}"
	fi
	if /bin/ls $REPO_HYPERONMC_FACTORY/periods.list >& /dev/null ; then
	    i=0
	    for line in $(cat $REPO_HYPERONMC_FACTORY/periods.list)
	    do
		if [ ! -z $line ] ; then
		    period=$(echo $line | awk -F: '{ print $1 }' )
		    if [ -z $period ] ; then 
			(1>&2 echo "$REPO_HYPERONMC_FACTORY/periods.list is wrongly formatted. Please check. Stopping...")
			(1>&2 echo "It should be like that: ")
			(1>&2 echo "period1:prefix1")
			(1>&2 echo "period2:prefix2")
			(1>&2 echo "period3:prefix3")
			return 1  
		    fi
		    prefix=$(echo $line | awk -F: '{ print $2 }' )
		    if [ -z $prefix ] ; then prefix='unknown'; (1>&2 echo "Unknown prefix for period $period"); fi
		    export hy_periods_names=$period:$hy_periods_names
		    export hy_periods_prefixes=$prefix:$hy_periods_prefixes
		    echo "I read period name $period, period prefix is $prefix">/dev/null
		    let i+=1
		fi
	    done
	    export hy_periods_number=$i
	    echo "I read $hy_periods_number periods">/dev/null
	else
            (1>&2 echo "no periods.list found in $REPO_HYPERONMC_FACTORY. Aborting")
            return 1
	fi
    fi
    #======== iterate over inputs and find period prefix for every input=================
    for input in "$@"
    do
	#echo 'input is' $input >/dev/null
	i=1
	while [ "$(echo $hy_periods_names | awk -F: -v pos=$i '{ print $pos }' )" != $input ] && [ $i -le $hy_periods_number ] ; do let i+=1 ; done
	if [ $i -gt $hy_periods_number ] ; then echo 'unknown' ;
	else echo $(echo $hy_periods_prefixes | awk -F: -v pos=$i '{ print $pos }' )
	fi    
    done
    return 0
}
export -f prefix_by_period	
#for i in  $(ls -d -1 20??-??); do ls $i/file_li*mm.dat ; done
#for i in $(ls  -1 2008-11/file_list*) ; do if [ $i = ${i/empty} ] && [ $i != ${i/nov} ] ; then echo $i ; fi ; done
function list_periods () {
    if prefix_by_period ; 
    then 
	i=1
	while [ $i -le "$hy_periods_number" ] ;    do echo $(echo $hy_periods_names | awk -F: -v pos=$i '{ print $pos }' ) ; let i+=1 ; done
    else 
	echo 'no periods' ; return 1;
    fi
    return 0
}
export -f list_periods
function period_by_prefix () {
    if prefix_by_period ; then
	for input in "$@"
	do
	    i=1
            while [ "$(echo $hy_periods_prefixes | awk -F: -v pos=$i '{ print $pos }' )" != $input ] && [ $i -le $hy_periods_number ] ; do let i+=1 ; done
            if [ $i -gt $hy_periods_number ] ; then echo 'unknown' ;
            else echo $(echo $hy_periods_names | awk -F: -v pos=$i '{ print $pos }' )
            fi
	done
    else
	echo 'no periods' ; return 1;
    fi
    return 0
}
export -f period_by_prefix
