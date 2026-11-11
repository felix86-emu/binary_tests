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
    local NAME="$1"
    local URL="https://cdn.felix86.com/tests/$NAME"
    local ETAG="$DIR/.$NAME.etag"
    echo "Checking $URL..."
    local CODE
    CODE=$(curl -fsSL --etag-compare "$ETAG" --etag-save "$ETAG.new" -o $DIR/downloading.zip -w '%{http_code}' "$URL")
    if [[ "$CODE" == "304" ]]; then
        echo "$NAME already at the latest version"
        rm -f "$ETAG.new" $DIR/downloading.zip
        return
    fi
    echo "Downloaded"
    echo "Unzipping..."
    unzip -qq -o -d $DIR $DIR/downloading.zip
    echo "Unzipped"
    rm $DIR/downloading.zip
    mv "$ETAG.new" "$ETAG"
}

download_and_unzip "felix86-binary-tests.zip"
download_and_unzip "libuv-tests.zip"
download_and_unzip "runtimes.zip"
download_and_unzip "signal_tests.zip"
download_and_unzip "bwrap.zip"
download_and_unzip "dav1d.zip"
