#!/bin/sh -x

if ./do_prebuilt.sh; then
    exit 0
else
    echo "No prebuilt artifacts, building from source"
fi

mkdir -p _build/cmake
cd _build/cmake

if type cmake3 > /dev/null 2>&1 ; then
    CMAKE=cmake3
else
    CMAKE=cmake
fi

# there are a few dependencies having cmake_minimum_required < 3.5
# deps/lz4/contrib/cmake_unofficial/CMakeLists.txt:36:1:cmake_minimum_required (VERSION 2.8.6)
# deps/snappy/third_party/benchmark/cmake/GoogleTest.cmake.in:1:1:cmake_minimum_required(VERSION 2.8.12)
# deps/snappy/CMakeLists.txt:29:1:cmake_minimum_required(VERSION 3.1)
${CMAKE} -DCMAKE_POLICY_VERSION_MINIMUM=3.5 "$@" ../../c_src || exit 1

echo done-do_cmake.
