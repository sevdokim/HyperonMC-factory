resubmit=yes
for cond in s4 #t2
do
    case "$cond" in
	"s4")     co=2 ;;
	"t2")     co=1 ;;
	*)        co=0 ;;
    esac
    for mes in rho0 #eta omg
    do   
	case "$mes" in
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
	for targ in be79mm c78mm al35mm cu7mm sn5mm pb3mm ch80mm
	do
	    case "$targ" in
		"be79mm")   export TARGET=2 ;;
		"c78mm")    export TARGET=10;;
		"al35mm")   export TARGET=3 ;;
		"al17mm")   export TARGET=3 ;;
		"cu3mm")    export TARGET=9 ;;
		"cu7mm")    export TARGET=9 ;;
		"sn5mm")    export TARGET=8 ;;
		"pb3mm")    export TARGET=7 ;;
		"ch80mm")   export TARGET=11;;
		*)          export TARGET=2 ;; #default value (Be)
	    esac
	    for i in {0..127}
	    do
		RUNNUMBER=$[ $i+$TARGET*10000+$n*1000000+$co*10000000 ]
		FILE=nov08_${targ}_${mes}_${cond}/MCruns/Run${RUNNUMBER}.gz
		if [ ! -f $FILE ] ; then
		    echo 'File ' $FILE 'does not exist!'
		    if [ $resubmit = yes ] ; then
			echo 'Resubmitting job'
			cd nov08_${targ}_${mes}_${cond}/nov08_${targ}_${mes}_${cond}_$i/
			qsub -q ihep-short ../../../generate_1target_qsub.sh
			cd -
		    fi
		else
		    FILESIZE=$(wc -c < $FILE)
		    #echo $FILESIZE
		    if [ $FILESIZE -lt 1500000 ] ; then
			echo 'Suspicious file ' $FILE 
			ls -lth $FILE
		    fi
		fi
	    done
	done
    done
done
