#!/bin/bash

FELIX=$1
DIR=$2
TEST=$3
SUCCESS_RET=$4

$1 $2 $3 $3 1> /dev/null 2> /dev/null
RET=$?

if [ $RET -ne $SUCCESS_RET ]; then
    echo "Test $TEST failed with $RET."
    exit 1
fi

echo "Test $TEST succeeded."
exit 0
