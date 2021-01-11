export start_dir=$(pwd)
export compress_dir
for compress_dir in $(ls -d -1 nov*)
do 
    cd $compress_dir
    qsub -q ihep-short ../compress_dir.sh
    cd ..
done
