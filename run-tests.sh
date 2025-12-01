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

xargs -P$(nproc) -I{} sh -c "./run-single-test.sh $FELIX $DIR/fex-gcc-target-tests-bins/ {} 0" < gcc_tests_must_pass.txt
RET=$?
if [ $RET -ne 0 ]; then
    echo "Failed some tests"
    exit 1
fi

xargs -P$(nproc) -I{} sh -c "./run-single-test-expect-fail.sh $FELIX $DIR/fex-gcc-target-tests-bins/ {} 0" < gcc_tests_known_failure.txt
RET=$?
if [ $RET -ne 0 ]; then
    echo "Failed some tests"
    exit 1
fi

xargs -P$(nproc) -I{} sh -c "./run-single-test.sh $FELIX $DIR/valgrind-tests-bins/ {} 0" < valgrind_tests_must_pass.txt
RET=$?
if [ $RET -ne 0 ]; then
    echo "Failed some tests"
    exit 1
fi

xargs -P$(nproc) -I{} sh -c "./run-single-test-expect-fail.sh $FELIX $DIR/valgrind-tests-bins/ {} 0" < valgrind_tests_known_failure.txt
RET=$?
if [ $RET -ne 0 ]; then
    echo "Failed some tests"
    exit 1
fi

# xargs -P$(nproc) -I{} sh -c "./run-single-test-arg.sh $FELIX $DIR/libuv-tests/uv_run_tests_a {} 0" < libuv_tests_must_pass.txt
# RET=$?
# if [ $RET -ne 0 ]; then
#     echo "Failed some tests"
#     exit 1
# fi

# xargs -P$(nproc) -I{} sh -c "./run-single-test-arg-expect-fail.sh $FELIX $DIR/libuv-tests/uv_run_tests_a {} 0" < libuv_tests_known_failures.txt
# RET=$?
# if [ $RET -ne 0 ]; then
#     echo "Failed some tests"
#     exit 1
# fi

echo "All tests passed!"
exit 0