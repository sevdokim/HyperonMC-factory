#!/bin/bash
#PBS -l ncpus=1
#PBS -l vmem=1000mb
#PBS -l cput=02:00:00
#PBS -l walltime=12:00:00
#PBS -q ihep-short
#PBS -V

compress_dir=$start_dir/$compress_dir
if [ ! -z $PBS_O_WORKDIR ] ; then compress_dir=$PBS_O_WORKDIR ; fi #if we in torque mode

echo 'base directory is' $start_dir
echo 'compress directory is' $compress_dir
cd $compress_dir

for ith in  $(ls -d -1 nov*)
do
    echo 'I  go in' $ith
    cd $ith
    if ls Run* >& /dev/null ; then #Run*.gz exists so this production is already finished
	echo 'I found Run.gz files. Removing them:'
	rm -fv Run*
	echo 'Cutting log_production and archiving generated files.'
	echo 'First 700 strings and last 700 strings of log_production. For full file look MC_res.dat.bz2 in generated.tar' > log_production_part
        head -n 700 log_production >> log_production_part
        echo '.................................................................' >> log_production_part
        echo '.............cutted here.........................................' >> log_production_part
        echo '.................................................................' >> log_production_part
        tail -n 700 log_production >> log_production_part
	bzip2 -z9 log_production 
	bzip2 -z9 MC_res.dat 
	bzip2 -z9 MCgen.dat
	if tar -cf generated.tar log_production.bz2 MC_res.dat.bz2 MCgen.dat.bz2 Histos.root ; then
	    echo 'Successfully archived!'
	    rm -vf log_production.bz2 MC_res.dat.bz2 MCgen.dat.bz2 Histos.root
	else
	    echo 'something went wrong!!!'
	fi
    else
	echo 'No Run.gz files. Check if compression is done.'
	recompress=no
	if [ -f MC_res.dat ]; then recompress=yes ; bzip2 -z9 MC_res.dat;  fi
	if [ -f log_production ]; then 
	    recompress=yes 
	    echo 'Cutting log_production and archiving generated files.'
	    echo 'First 700 strings and last 700 strings of log_production. For full file look MC_res.dat.bz2 in generated.tar' > log_production_part
	    head -n 700 log_production >> log_production_part
	    echo '.................................................................' >> log_production_part
	    echo '.............cutted here.........................................' >> log_production_part
	    echo '.................................................................' >> log_production_part
	    tail -n 700 log_production >> log_production_part		
	    bzip2 -z9 log_production ; fi
	if [ -f MCgen.dat ]; then recompress=yes ; bzip2 -z9 MCgen.dat; fi
	if [ -f Histos.root ]; then recompress=yes ; fi
	if ls *.bz2 >& /dev/null; then recompress=yes; fi
	if [ recompress = yes ]; then
	    echo 'Recompressing...'
	    if [[ -f log_production.bz2 ]] && [[ -f MC_res.dat.bz2 ]] && [[ -f MCgen.dat.bz2 ]] && [[ -f Histos.root ]] ; then 
		tar -cf generated.tar log_production.bz2 MC_res.dat.bz2 MCgen.dat.bz2 Histos.root
	    else
		echo 'Recompression failed in' $ith 1>&2
		echo 'Looks like we missed some files...'
	    fi
	fi
    fi
    cd ..
done
