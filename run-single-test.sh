#!/bin/bash

FELIX=$1
DIR=$2
TEST=$3
SUCCESS_RET=$4

export TEST_TMPDIR=$(mktemp -d)
OUTPUT_FILE="$TEST_TMPDIR.out"
$FELIX $DIR/$TEST > "$OUTPUT_FILE" 2>&1
RET=$?
rm -rf "$TEST_TMPDIR"

if [ $RET -ne $SUCCESS_RET ]; then
    echo "Test $TEST failed with $RET."
    echo "Output:"
    cat "$OUTPUT_FILE"
    rm -f "$OUTPUT_FILE"
    exit 1
fi

rm -f "$OUTPUT_FILE"
echo "Test $TEST succeeded."
exit 0