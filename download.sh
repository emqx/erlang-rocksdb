#!/bin/sh

set -eu
set -x

TAG="$(git describe --tags | head -1)"
PKGNAME="$(./pkgname.sh)"
URL="https://github.com/emqx/erlang-rocksdb/releases/download/$TAG/$PKGNAME"

if [ ! -f "priv/${PKGNAME}" ]; then
    curl -f -L -o "priv/${PKGNAME}" "${URL}"
fi

if [ ! -f "priv/${PKGNAME}.sha256" ]; then
    curl -f -L -o "priv/${PKGNAME}.sha256" "${URL}.sha256"
fi

echo "$(cat "priv/${PKGNAME}.sha256") priv/${PKGNAME}" | sha256sum -c || exit 1

gzip -c -d "priv/${PKGNAME}" > priv/liberocksdb.so
