#!/bin/sh

set -eu
set -x

PKGNAME="$(./pkgname.sh)"

if [ "${BUILD_RELEASE:-}" != 1 ] && [ -n "$PKGNAME" ]; then
    if ./download.sh $PKGNAME; then
        echo "done_dowloading_rocksdb"
        exit 0
    else
        echo "failed to download, continue to build from source"
    fi
fi

cd _build/cmake

if type cmake3 > /dev/null 2>&1 ; then
    CMAKE=cmake3
else
    CMAKE=cmake
fi

case "$@" in
    *-j*)
        ${CMAKE} --build . -- "$@" || exit 1
        ;;
    *)
        CORES=$(getconf _NPROCESSORS_ONLN 2>/dev/null || sysctl -n hw.ncpu)

        if [ "x$CORES" = "x" ]; then
            PAR=""
        else
            PAR="-- -j $CORES"
        fi
        ${CMAKE} --build . $PAR $@ || exit 1
        ;;
esac

cd ../../

if [ "${BUILD_RELEASE:-}" = 1 ]; then
    if [ -z "$PKGNAME" ]; then
        echo "unable_to_resolve_release_package_name"
        exit 1
    fi
    mkdir -p _packages
    TARGET="_packages/${PKGNAME}"
    gzip -c 'priv/liberocksdb.so' > "$TARGET"
    # use openssl but not sha256sum command because in some macos env it does not exist
    if command -v openssl; then
        openssl dgst -sha256 "${TARGET}" | cut -d ' ' -f 2  > "${TARGET}.sha256"
    else
        sha256sum "${TARGET}"  | cut -d ' ' -f 1 > "${TARGET}.sha256"
    fi
fi

echo done_building_rocksdb
