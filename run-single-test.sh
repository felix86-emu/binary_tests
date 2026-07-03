#!/bin/bash

FELIX=$1
DIR=$2
TEST=$3
SUCCESS_RET=$4

OUTPUT=$($FELIX $DIR/$TEST 2>&1)
RET=$?

if [ $RET -ne $SUCCESS_RET ]; then
    echo "Test $TEST failed with $RET."
    echo "Output:"
    echo "$OUTPUT"
    exit 1
fi

echo "Test $TEST succeeded."
exit 0