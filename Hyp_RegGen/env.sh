if ls $MYSW/env.sh >& /dev/null ; then
  source $MYSW/env.sh
else
  return 1
fi
