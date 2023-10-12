#!/bin/sh

set -eu

if [ ! -f priv/liberocksdb.so ]; then

PKGNAME="$(./pkgname.sh)"
if [ "${BUILD_RELEASE:-}" != 1 ] && [ -n "$PKGNAME" ]; then
    if ./download.sh $PKGNAME; then
        exit 0
    else
        exit 1
    fi
fi

fi

# Sanity check
erlc src/rocksdb.erl
trap "rm -f rocksdb.beam" EXIT
if erl -noshell -eval '[_|_]=rocksdb:module_info(), halt(0)'; then
    exit 0
else
    exit 1
fi
