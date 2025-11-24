#!/bin/bash

FELIX=$1
DIR=$2
TEST=$3

$1 $2/$3 1> /dev/null 2> /dev/null
RET=$?

if [ $RET -ne 0 ]; then
    echo "Test $TEST failed with $RET."
    exit 1
fi

echo "Test $TEST succeeded."
exit 0
