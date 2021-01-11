for w in {8..15}
do
    for i in {0..263}
    do
	RUNNUMBER=$[ 1100000+2000+1000*$w+$i ]
	FILE=nov08_be79mm_omg_width${w}MeV/MCruns/Run${RUNNUMBER}.gz
	if [ ! -f $FILE ] ; then
	    echo 'File ' $FILE 'does not exist!'
	else
	    FILESIZE=$(wc -c < $FILE)
	    #echo $FILESIZE
	    if [ $FILESIZE -lt 3000000 ] ; then
		echo 'Suspicious file ' $FILE 
		ls -lth $FILE
	    fi
	fi
    done
done
