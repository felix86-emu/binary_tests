#!/bin/bash

FELIX=$1
DIR=$2
TEST=$3
SUCCESS_RET=$4

export TEST_TMPDIR=$(mktemp -d)
$1 $2/$3 1> /dev/null 2> /dev/null
RET=$?
rm -rf "$TEST_TMPDIR"

if [ $RET -eq $SUCCESS_RET ]; then
    echo "Test $TEST succeeded with $RET but I expected it to fail."
    exit 1
fi

echo "Test $TEST failed as expected."
exit 0
