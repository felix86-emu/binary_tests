#!/bin/bash

FELIX="$1"
DIR="$2"

if [ -z "$DIR" ] || [ -z "$FELIX" ]; then
    echo "Usage: $0 <path to felix86> <path to tests>"
    echo "Example: $0 /opt/felix86/felix86 /opt/felix86/tests"
    exit 1
fi

if [ ! -d "$DIR" ]; then
    echo "Directory $DIR doesn't exist."
    exit 1
fi

if [ ! -x "$FELIX" ]; then
    echo "felix86 at $FELIX doesn't exist or isn't executable"
    exit 1
fi

xargs -P$(nproc) -I{} sh -c "./run-single-test.sh $FELIX $DIR/fex-gcc-target-tests-bins/{}" < gcc_tests_must_pass.txt
RET=$?
if [ $RET -ne 0 ]; then
    echo "Failed some tests"
    exit 1
fi

echo "All tests passed!"
exit 0