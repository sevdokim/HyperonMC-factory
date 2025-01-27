if ls $MYSW/env.sh >& /dev/null ; then
  source $MYSW/env.sh
else
  exit 1
fi