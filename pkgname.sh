#!/bin/sh

if [ ! -d .git ]; then
    # not a clone
    echo ''
    exit 0
fi

UNAMES="$(uname -s)"
case "$UNAMES" in
    Darwin)
        DIST='macos'
        VERSION_ID="$(sw_vers | grep 'ProductVersion' | cut -d':' -f 2 | cut -d'.' -f1 | tr -d ' \t')"
        SYSTEM="${DIST}${VERSION_ID}"
        ;;
    Linux)
        if grep -q -i 'rhel' /etc/*-release; then
            DIST='el'
            VERSION_ID="$(rpm --eval '%{rhel}')"
        else
            DIST="$(sed -n '/^ID=/p' /etc/os-release | sed -r 's/ID=(.*)/\1/g' | sed 's/"//g')"
            VERSION_ID="$(sed -n '/^VERSION_ID=/p' /etc/os-release | sed -r 's/VERSION_ID=(.*)/\1/g' | sed 's/"//g')"
        fi
        SYSTEM="$(echo "${DIST}${VERSION_ID}" | sed -r 's/([a-zA-Z]*)-.*/\1/g')"
        ;;
    *)
        # unsupported system
        echo ''
        exit 0
        ;;
esac

ARCH="$(uname -m)"
VSN="$(git describe --tags --exact-match | head -1)"

if [ -z "$VSN" ]; then
    VSN="debug"
fi

OTP="$(erl -noshell -eval 'io:format(erlang:system_info(otp_release)).' -s init stop)"

echo "liberocksdb-${VSN}-otp${OTP}-${SYSTEM}-${ARCH}.gz"
