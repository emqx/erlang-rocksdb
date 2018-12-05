

# erlang-rocksdb - Erlang wrapper for RocksDB. #

Copyright (c) 2016-2018 Benoît Chesneau.

__Version:__ 0.26.0 Erlang wrapper for RocksDB.

Feedback and pull requests welcome! If a particular feature of RocksDB is important to you, please let me know by opening an issue, and I'll prioritize it.

## Features

- rocksdb 5.17.2 with snappy 1.1.7, lz4 1.8.3
- Erlang 19.3 and sup with dirty-nifs enabled
- all basics db operations
- batchs support
- snapshots support
- checkpoint support
- column families support
- transaction logs
- backup support
- erlang merge operator
- customized build support
- Tested on macosx, freebsd, solaris and linux

## Usage

See the [Gitlab Wiki](https://gitlab.com/barrel-db/erlang-rocksdb/wikis/home) for more explanation.

## Customized build ##

See the [Customized builds](http://gitlab.com/barrel-db/erlang-rocksdb/blob/master/doc/customize_rocksdb_build.md) for more information.

## Support

Support, Design and discussions are done via the [Gitlab Tracker](https://gitlab.com/barrel-db/erlang-rocksdb/issues) or [email](mailto:incoming+barrel-db/erlang-rocksdb@gitlab.com).

## License

Erlang RocksDB is licensed under the Apache License 2.


## Modules ##


<table width="100%" border="0" summary="list of modules">
<tr><td><a href="http://gitlab.com/barrel-db/erlang-rocksdb/blob/master/doc/rocksdb.md" class="module">rocksdb</a></td></tr>
<tr><td><a href="http://gitlab.com/barrel-db/erlang-rocksdb/blob/master/doc/rocksdb_bump.md" class="module">rocksdb_bump</a></td></tr>
<tr><td><a href="http://gitlab.com/barrel-db/erlang-rocksdb/blob/master/doc/rocksdb_sst_file_manager.md" class="module">rocksdb_sst_file_manager</a></td></tr>
<tr><td><a href="http://gitlab.com/barrel-db/erlang-rocksdb/blob/master/doc/rocksdb_write_buffer_manager.md" class="module">rocksdb_write_buffer_manager</a></td></tr></table>

