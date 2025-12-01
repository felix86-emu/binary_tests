#!/bin/bash

set -e
DIR="$1"

if [ -z "$DIR" ]; then
    echo "Usage: $0 <directory to install tests>"
    echo "Example: $0 /opt/felix86/tests"
    exit 1
fi

if [ -d "$DIR" ]; then
    echo "Directory $DIR already exists. Remove it and all its contents to install the tests."
    exit 1
fi

mkdir -p /tmp/felix86-tests
curl -k -L https://cdn.felix86.com/tests/felix86-binary-tests.zip -o /tmp/felix86-tests/felix86-binary-tests.zip
mkdir -p $DIR
unzip -qq -o -d $DIR /tmp/felix86-tests/felix86-binary-tests.zip
mv $DIR/binary_tests-main/* $DIR/
curl -k -L http://cdn.felix86.com/tests/libuv-tests.zip -o /tmp/felix86-tests/libuv-tests.zip
unzip -qq -o -d $DIR /tmp/felix86-tests/libuv-tests.zip
rmdir $DIR/binary_tests-main
rm /tmp/felix86-tests/felix86-binary-tests.zip
rm /tmp/felix86-tests/libuv-tests.zip
rmdir /tmp/felix86-tests
