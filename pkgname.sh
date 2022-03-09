#!/bin/sh

UNAMES="$(uname -s)"
case "$UNAMES" in
    Darwin)
        DIST='macos'
        VERSION_ID=$(sw_vers | gsed -n '/^ProductVersion:/p' | gsed -r 's/ProductVersion:(.*)/\1/g' | gsed -r 's/([0-9]+).*/\1/g' | gsed 's/^[ \t]*//g')
        SYSTEM="$(echo "${DIST}${VERSION_ID}" | gsed -r 's/([a-zA-Z]*)-.*/\1/g')"
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
        echo ''
        ;;
esac

ARCH="$(uname -m)"
VSN="$(git describe --tags | head -1)"

OTP="$(erl -noshell -eval 'io:format(erlang:system_info(otp_release)).' -s init stop)"

echo "liberocksdb-${VSN}-otp${OTP}-${SYSTEM}-${ARCH}.gz"
