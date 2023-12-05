#!/bin/sh

set -eu

if [ "${BUILD_RELEASE:-}" = 1 ]; then
    # never download when building a new release
    exit 1
fi

if [ ! -f priv/liberocksdb.so ]; then
    PKGNAME="$(./pkgname.sh)"
    if [ -n "$PKGNAME" ]; then
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
