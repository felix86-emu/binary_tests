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

current_dir=$(pwd)

xargs -P$(nproc) -I{} sh -c "$current_dir/run-single-test.sh $FELIX $DIR/fex-gcc-target-tests-bins/ {} 0" < "$current_dir/gcc_tests_must_pass.txt"
RET=$?
if [ $RET -ne 0 ]; then
    echo "Failed some tests"
    exit 1
fi

xargs -P$(nproc) -I{} sh -c "$current_dir/run-single-test-expect-fail.sh $FELIX $DIR/fex-gcc-target-tests-bins/ {} 0" < "$current_dir/gcc_tests_known_failure.txt"
RET=$?
if [ $RET -ne 0 ]; then
    echo "Failed some tests"
    exit 1
fi

xargs -P$(nproc) -I{} sh -c "$current_dir/run-single-test.sh $FELIX $DIR/valgrind-tests-bins/ {} 0" < "$current_dir/valgrind_tests_must_pass.txt"
RET=$?
if [ $RET -ne 0 ]; then
    echo "Failed some tests"
    exit 1
fi

xargs -P$(nproc) -I{} sh -c "$current_dir/run-single-test-expect-fail.sh $FELIX $DIR/valgrind-tests-bins/ {} 0" < "$current_dir/valgrind_tests_known_failure.txt"
RET=$?
if [ $RET -ne 0 ]; then
    echo "Failed some tests"
    exit 1
fi

# cd "$DIR/libuv-tests"

# xargs -P$(nproc) -I{} sh -c "$current_dir/run-libuv-test.sh $FELIX $DIR/libuv-tests/uv_run_tests_a {} 0" < "$current_dir/libuv_tests_must_pass.txt"
# RET=$?
# if [ $RET -ne 0 ]; then
#     echo "Failed some tests"
#     exit 1
# fi

# xargs -P$(nproc) -I{} sh -c "$current_dir/run-libuv-test-expect-fail.sh $FELIX $DIR/libuv-tests/uv_run_tests_a {} 0" < "$current_dir/libuv_tests_known_failures.txt"
# RET=$?
# if [ $RET -ne 0 ]; then
#     echo "Failed some tests"
#     exit 1
# fi

# cd "$current_dir"

# Run the runtime tests

echo "Running JAR test..."
export FELIX86_QUIET=1
JAR_OUTPUT=$(cd $DIR/jdk && $FELIX $DIR/jdk/bin/java -jar ./helloworld.jar) 
RET=$?
if [ $RET -ne 0 ]; then
    echo "Failed some tests"
    exit 1
fi

if [ "$JAR_OUTPUT" != "Hello world from HelloWorld.jar!" ]; then
    echo "Failed some tests"
    echo "JAR returned: $JAR_OUTPUT"
    exit 1
else
    echo "JAR test passed"
fi

echo "Running Node test..."
NODE_OUTPUT=$($FELIX $DIR/node/bin/node -e "console.log(\"hello\")")
RET=$?
if [ $RET -ne 0 ]; then
    echo "Failed some tests"
    exit 1
fi

if [ "$NODE_OUTPUT" != "hello" ]; then
    echo "Failed some tests"
    echo "Node returned: $NODE_OUTPUT"
    exit 1
else
    echo "Node test passed"
fi

echo "All tests passed!"
exit 0