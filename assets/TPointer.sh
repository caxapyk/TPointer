#!/bin/sh

##### Connection #####
server="localhost"
db="tpointer"
user="tpointer"
password="password"
######################

appname=`basename $0 | sed s,\.sh$,,`

dirname=`dirname $0`
tmp="${dirname#?}"

if [ "${dirname%$tmp}" != "/" ]; then
dirname=$PWD/$dirname
fi
LD_LIBRARY_PATH=$dirname
export LD_LIBRARY_PATH
$dirname/$appname "-h ${server} -d ${db} -u ${user} -p ${password} $@"
