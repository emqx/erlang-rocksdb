

# erlang-rocksdb - HTTP client library in Erlang #

Copyright (c) 2016 Benoît Chesneau.

__Version:__ 0.6.0 Erlang wrapper for RocksDB.

Feedback and pull requests welcome! If a particular feature of RocksDB is important to you, please let me know by opening an issue, and I'll prioritize it.

### Examples

```
{ok, Db} = erocksdb:open("path/for/rocksdb/storage", []),
erocksdb:put(Db, <<"my key">>, <<"my value">>),
case erocksdb:get(Db, <<"my key">>, []) of
  {ok, Value} => io:format("retrieved value %p~n", [Value]);
  not_found => io:format("value not found~n", []);
  Error -> io:format("operational problem encountered: %p~n", [Error])
end,
erocksdb:close(Db),
erocksdb:destroy(Db).
```

# Features

- rocksdb 4.13
- all basics db operations
- snapshots support
- checkpoint support
- column families support
- Tested on macosx and

## Notes

This project is a fork of [erocksdb](https://github.com/leo-project/erocksdb) sponsored by Enki Multimedia (https://enkim.eu).


## Modules ##


<table width="100%" border="0" summary="list of modules">
<tr><td><a href="erocksdb.md" class="module">erocksdb</a></td></tr>
<tr><td><a href="erocksdb_bump.md" class="module">erocksdb_bump</a></td></tr></table>

