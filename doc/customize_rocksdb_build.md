# Customized builds #

Since version 0.26.0 Erlang wrapper for RocksDB support customized build.

There are three options added to customize build (all are `OFF` by default):

* `ERLANG_ROCKSDB_USE_SYSTEM_ROCKSDB` then `ON` the driver will link with RocksDB shared library provided by host system

* `ERLANG_ROCKSDB_USE_SYSTEM_SNAPPY` then `ON` the driver will link with snappy shared library provided by host system

* `ERLANG_ROCKSDB_USE_SYSTEM_LZ4` then `ON` the driver will link with lz4 shared library provided by host system

All options passes via `ERLANG_ROCKSDB_OPTS` environment variable or via `rebar.config.script`. For example, command

```
 $ export ERLANG_ROCKSDB_OPTS="-DERLANG_ROCKSDB_USE_SYSTEM_ROCKSDB=ON -DERLANG_ROCKSDB_USE_SYSTEM_SNAPPY=ON -DERLANG_ROCKSDB_USE_SYSTEM_LZ4=ON"
 $ make
```

gives following output on macOS

```
===> Verifying dependencies...
===> Compiling rocksdb
-- Using OTP lib: /usr/local/Cellar/erlang/21.1.4/lib/erlang/lib - found
-- Using erl_interface version: erl_interface-3.10.4
-- Using erts version: erts-10.1.3
-- Found LZ4: /usr/local/lib/liblz4.dylib
-- Found SNAPPY: /usr/local/lib/libsnappy.dylib
-- Found RocksDB: /usr/local/lib/librocksdb.dylib (found suitable version "5.17.2", minimum required is "5.17.2")
-- Configuring done
-- Generating done

...
[SKIP BUILD OUTPUT]
...

```