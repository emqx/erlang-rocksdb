#!/bin/sh

set -eu

TAG="$(git describe --tags | head -1)"
PKGNAME="$(./pkgname.sh)"
URL="https://github.com/emqx/erlang-rocksdb/releases/download/$TAG/$PKGNAME"

mkdir -p _packages
if [ ! -f "_packages/${PKGNAME}" ]; then
    curl -f -L -o "_packages/${PKGNAME}" "${URL}"
fi

if [ ! -f "_packages/${PKGNAME}.sha256" ]; then
    curl -f -L -o "_packages/${PKGNAME}.sha256" "${URL}.sha256"
fi

echo "$(cat "_packages/${PKGNAME}.sha256") _packages/${PKGNAME}" | sha256sum -c || exit 1

gzip -c -d "_packages/${PKGNAME}" > priv/liberocksdb.so

erlc src/rocksdb.erl
if erl -noshell -eval '[_|_]=rocksdb:module_info(), halt(0)'; then
    res=0
else
    res=1
fi
rm -f rocksdb.beam
exit $res
