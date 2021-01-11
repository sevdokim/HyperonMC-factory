THIS_FILE_NAME=$(echo $BASH_SOURCE | awk -F/ '{print $NF}')
THIS_PATH=${BASH_SOURCE%$THIS_FILE_NAME}
if [ -z $THIS_PATH ] ; then THIS_PATH=$(pwd)'/' ; fi
if [ -z $REPO_HYPERONMC_FACTORY ] ; then 
    REPO_PATH=$THIS_PATH
else
    REPO_PATH=$REPO_HYPERONMC_FACTORY
fi
if cd $REPO_PATH && /bin/ls repo-name && [ $(cat repo-name) = hyperonmc-factory ] ; then
    if [ -e config/config.$THIS_COMPUTER_NAME ] ; then
	for var in $(cat config/config.$THIS_COMPUTER_NAME)
	do
	    echo "HyperonMC-factory: start: I read $var from config/config.$THIS_COMPUTER_NAME"
	    export $var
	done
    else
	echo "HyperonMC-factory: start: No congig/config.$THIS_COMPUTER_NAME found. Creating"
	echo "REPO_HYPERONMC_FACTORY=$PWD" > config/config.$THIS_COMPUTER_NAME
    fi
    source period_prefix.sh
    cd -
    return 0
else
    echo "HyperonMC-factory: start: no repo found in $REPO_HYPERONMC_FACTORY"
    return 1
fi
