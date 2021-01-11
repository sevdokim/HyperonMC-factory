#!/bin/bash
#PBS -l ncpus=1
#PBS -l mem=500mb
#PBS -l vmem=1000mb
#PBS -l cput=96:00:00
#PBS -l walltime=96:00:00
#PBS -q alice
#PBS -V

# ^^^^^^^^ служебные вещи для qsub, они походу не работают, но это не важно.

######################################################################################################
# небольшая защита от дурака, by S.E. чтобы не было пустых значений (хотя их быть и не должно)
if [ -z $EXTARGET ];    then  export EXTARGET=-1;    fi; 
if [ -z $EXRESON ] ;    then  export EXRESON=-1;     fi;
if [ -z $EXCHANEL ];    then  export EXCHANEL=-1;    fi;
if [ -z $CONTROL ];     then  export CONTROL=0;      fi;
if [ -z $SEED ];        then  export SEED=$(cat /dev/urandom | tr -dc '0-9' | head --bytes 8) ;  fi; # true random 
if [ -z $TARGET ];      then  export TARGET=0;       fi;
if [ -z $EVENTNUMBER ]; then  export EVENTNUMBER=10; fi;
if [ -z $WD ];          then  echo 'you are doing wrong!';  echo 'export working directory WD first';                    return; fi;
if [ -z $MACRODIR ];    then  echo 'you are doing wrong!';  echo 'export directory with running macros MACRODIR first';  return; fi;
if [ -z $FILELIST ];    then  echo 'you are doing wrong!';  echo 'export filelist name FILELIST first';                  return; fi;
if [ -z $SUFFIX ];      then  echo 'you are doing wrong!';  echo 'export name of directory where to generate results SUFFIX first';                return; fi;
if [ -z $ANCHORS ];     then  echo 'you are doing wrong!';  echo 'export ANCHORS directory where you store calibr.cards and file_list.dat first';  return; fi;
######################################################################################################

#вывод инфо
echo 'current WD='$WD
echo 'macro dir is ' $MACRODIR
echo 'suffix which will be used as new directory name is ' $SUFFIX
echo 'filelist to be used: ' $(ls $FILELIST)
echo 'calibr.cards to be used: ' $(ls $ANCHORS/calibr.cards)
echo 'number of events to be generated ' $EVENTNUMBER
echo 'target code is ' $TARGET
echo 'exclusive target is ' $EXTARGET
echo 'exclusive reson is ' $EXRESON
echo 'exclusive chanel is ' $EXCHANEL
echo 'generator control parameter is ' $CONTROL 
echo 'seed = ' $SEED
echo 'starting generation...'

source $MACRODIR/vmc_env.sh

LD_LIBRARY_PATH=$MCDIR/lib/tgt_linuxx8664gcc/:$LD_LIBRARY_PATH
#export PYTHIA8DATA=/data3/users/evdokimov/hyperon/analysis/pythia8/pythia4hyperon/PYTHIA8/pythia8210/share/Pythia8/xmldoc/

cd $WD
#создаём директорию
mkdir -p $SUFFIX
cd $SUFFIX
# копируем туда всё, что нужно
cp $MACRODIR/RegGen/RegGen.cards ./ # тут тоже можно прописать любой путь

cp $MACRODIR/run_g3_control.C ./
cp $MACRODIR/g3tgeoConfig.C ./
#cp $MACRODIR/macro/ ./ -r
ln -s $MACRODIR/macro/ ./  #  линк, дабы не плодить сущностей. остальное, в принципе, можно и поправить на месте и перезапустить, потому для остального пусть останется копирование.
cp $FILELIST ./file_list.dat
cp $ANCHORS/calibr.cards ./
cp $ANCHORS/h_s_new.dat ./
cp $ANCHORS/coeff_old.dat ./
# запускаем МК
root -b -q run_g3_control.C\($TARGET,$EVENTNUMBER,\"\",$EXTARGET,$EXRESON,$EXCHANEL,$CONTROL,$SEED\);
#ln -s MC_res${SUFFIX}.dat MC_res.dat # удобная ссылочка
# cp $MACRODIR/converter/exe .            #копируем конвертер
ln -s $MACRODIR/converter/exe ./converter  #линкуем конвертер
#./exe MC_res.dat $SEED > log_converter_$SEED # можно его тут запустить, в принципе.
#возвращаемся обратно, перемещаем себя.
cd $WD
mv $SUFFIX.sh ./$SUFFIX/;
echo; echo ; echo "now try:";
echo "cd $SUFFIX; ./converter MC_res.dat $SEED > log_converter_$SEED # you can choose any number here";
echo;
