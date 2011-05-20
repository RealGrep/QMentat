#!/bin/bash
#abspath=$(cd ${0%/*} && echo $PWD/${0##*/})
# to get the path only - not the script name - add
#path_only=`dirname "$abspath"`
DIR="$( cd "$( dirname "$0" )" && pwd )"

export LD_LIBRARY_PATH=$DIR/lib/
cd $DIR
./QMentat
