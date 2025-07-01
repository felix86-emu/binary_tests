#!/bin/bash

FELIX=$1
TEST=$2

$1 $2
RET=$?

if [ $RET -ne 0 ]; then
    name=$(basename "$TEST")
    echo "Test $name failed with $RET."
    exit 1
fi

exit 0
