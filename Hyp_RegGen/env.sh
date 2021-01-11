#!/bin/bash

export number="0001"; #это список номеров для имени директории (пока что один). можно написать несколько для распараллеливания, или задать из ком.строки.
export EVENTNUMBER=101 #number of events to generate in one production thread.
# это коренная директория,в которой лежит скомпилированное МК. в ней находятся папки: HyMC_RegGen  lib  tmp . Нужно поменять это имя на то, в которой будет ваше МК.
export MCDIR=/data3/users/kondratyuk/hyperon/RegGen/v3.02_for_SA/ ;
export WD=$PWD # WD is working directory. в ней создастся папка с результатами МК. может быть любой, пусть будет данная.
# ANDIR dir is where you store file_list.dat, calibr.cards, h_s_new.dat and coefs_old.dat
export ANDIR=/home/kondratyuk/hyperon/sessions/analyzer_1_2_8/  # тут мои конфиги, путь можно поменять.
export PERIOD=2008-11
export ANCHORS=$ANDIR/$PERIOD/config_save/ # папка с конфигами от обработки типа calibr.cards, h_s_new.dat and coefs_old.dat
export FILELIST=$ANDIR/$PERIOD/file_lists/file_list_nov08_be79mm_allstat.dat  #file list taken from $ANCHORS. You can put full path to file_list.dat instead.

#choose exclusive target (1 = S4, 2 = target, 3 = Sa, 0 = all of them, -1 => value is taken from hyp_carlo.f code)
export EXTARGET=0   
###################################################
export TARGET=2   #Be target
#  // Be = 2
#  // C  = 10
#  // Al = 3
#  // Cu = 9
#  // Sn = 8
#  // Pb = 7
#  // Poly = 11
###################################################
if [ "$#" -gt "0" ];  then export number="$@"; fi;  # "$#" - number of input parameters. задаём имена директорий из командной строки.


# тут что-то для пифии и прописываются пути к МК
export MACRODIR=$MCDIR/HyMC_RegGen/        # MC needs some macros and other stuff. This is HyMC_devel directory. менять не нужно
LD_LIBRARY_PATH=.:..:/data3/users/evdokimov/hyperon/analysis/pythia8/pythia4hyperon/PYTHIA8/pythia8210/lib/:$MCDIR/lib/tgt_linuxx8664gcc/:$LD_LIBRARY_PATH
export PYTHIA8DATA=/data3/users/evdokimov/hyperon/analysis/pythia8/pythia4hyperon/PYTHIA8/pythia8210/share/Pythia8/xmldoc/
#some dummy variables - рудименты, лучше оставить как есть. EXRESON и EXCHANEL более не используются, вместо них реакции берутся из reggen.cards
export EXRESON=-1
export EXCHANEL=-1
export CONTROL=0

export SEED=$(cat /dev/urandom | tr -dc '0-9' | head --bytes 8) ; # true random
echo "$number "'SEED='$SEED;   
export SUFFIX=mc_$number # имя директории, в которой будут данные, можно прописать полный путь.
source $MACRODIR/vmc_env.sh
LD_LIBRARY_PATH=$MCDIR/lib/tgt_linuxx8664gcc/:$LD_LIBRARY_PATH


