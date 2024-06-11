#export MYSW=/data/mysw
#source $MYSW/env.sh
export PYTHIA8=$MYSW/INSTALL/pythia8
export PYTHIA8DATA=$PYTHIA8/share/Pythia8/xmldoc/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PYTHIA8/lib
script_dir=$(dirname $(readlink -f $0))
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$script_dir/../install/lib
