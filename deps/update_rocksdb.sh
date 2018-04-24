#!/bin/bash
VERSION=$1
URL="https://github.com/facebook/rocksdb/archive/v$VERSION.tar.gz"
echo "Downloading $URL"
curl -o /tmp/rocksdb.tar.gz -L $URL
tar xvzf /tmp/rocksdb.tar.gz
rm -rf rocksdb
mv rocksdb-${VERSION} rocksdb
echo "Use git status, add all files and commit changes."
