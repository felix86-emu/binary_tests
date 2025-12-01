#!/bin/bash

set -e
DIR="$1"

if [ -z "$DIR" ]; then
    echo "Usage: $0 <directory to install tests>"
    echo "Example: $0 /opt/felix86/tests"
    exit 1
fi

mkdir -p $DIR

download_and_unzip() {
    local URL="$1"
    echo "Downloading $URL..."
    curl -k -L "$URL" -o $DIR/downloading.zip
    echo "Downloaded"
    echo "Unzipping..."
    unzip -qq -o -d $DIR $DIR/downloading.zip
    echo "Unzipped"
    rm $DIR/downloading.zip
}

SHOULD_DOWNLOAD=0
echo "Getting version.txt..."
curl -sL https://cdn.felix86.com/tests/version.txt > $DIR/version.txt.tmp
if [[ ! -f "$DIR/version.txt" ]]; then
    echo "First time downloading test binaries!"
    SHOULD_DOWNLOAD=1
else
    if [[ "$(cat "$DIR/version.txt.tmp")" != "$(cat "$DIR/version.txt")" ]]; then
        echo "Test binary version changed, starting new download"
        SHOULD_DOWNLOAD=1
    fi
fi

if [[ $SHOULD_DOWNLOAD -eq 1 ]]; then
    download_and_unzip "https://cdn.felix86.com/tests/felix86-binary-tests.zip"
    download_and_unzip "https://cdn.felix86.com/tests/libuv-tests.zip"
    mv $DIR/version.txt.tmp $DIR/version.txt
else
    echo "Test binaries already at the latest version!"
    rm $DIR/version.txt.tmp
fi