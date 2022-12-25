#shell script for vmc environment variables

#path for all programs you need
#export VMC=/afs/ihep.su/user/e/ekondrat/_mc_source/
# export VMC=/afs/ihep.su/sw/hyperon/_mc_source/
#export VMC=/afs/ihep.su/sw/hyperon/_mc_source/
export VMC=/afs/ihep.su/sw/hyperon/vmc/


#path for root
#export ROOTSYS=$VMC/root
export ROOTSYS=$VMC/root_v5.28.00_rebuild
source $ROOTSYS/bin/thisroot.sh
#PATH=$ROOTSYS/bin:$PATH
#export LD_LIBRARY_PATH=$ROOTSYS/lib

#path for g3
LD_LIBRARY_PATH=$VMC/geant3_rebuild/lib/tgt_linuxx8664gcc/:$LD_LIBRARY_PATH

#path for g4
#LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$VMC/CLHEP/lib
#LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$VMC/geant4.9.4.p01/lib/Linux-g++/
#source $VMC/geant4.9.4.p01/env.sh > /dev/null

#path for g4_vmc and vmc examples
#LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$VMC/geant4_vmc/lib/tgt_linuxx8664gcc/

#path for system libs
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu/
