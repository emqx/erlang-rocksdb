

# erlang-rocksdb - Erlang wrapper for RocksDB. #

Copyright (c) 2016-2017 Benoît Chesneau.

__Version:__ 0.9.0 Erlang wrapper for RocksDB.

Feedback and pull requests welcome! If a particular feature of RocksDB is important to you, please let me know by opening an issue, and I'll prioritize it.

## Features

- rocksdb 5.4.5
- Erlang 19.3 and sup with dirty-nifs enabled
- all basics db operations
- batchs support
- snapshots support
- checkpoint support
- column families support
- transaction logs
- Tested on macosx, freebsd, solaris and linux

## Usage examples

some snippets extracted from tests

### Simple usage

```
{ok, Db} = rocksdb:open("path/for/rocksdb/storage", []),
rocksdb:put(Db, <<"my key">>, <<"my value">>),
case rocksdb:get(Db, <<"my key">>, []) of
  {ok, Value} => io:format("retrieved value %p~n", [Value]);
  not_found => io:format("value not found~n", []);
  Error -> io:format("operational problem encountered: %p~n", [Error])
end,
rocksdb:close(Db),
rocksdb:destroy(Db).
```

### Fold kvs

```
{ok, Ref} = rocksdb:open("/tmp/erocksdb.fold.test", [{create_if_missing, true}]),
ok = rocksdb:put(Ref, <<"def">>, <<"456">>, []),
ok = rocksdb:put(Ref, <<"abc">>, <<"123">>, []),
ok = rocksdb:put(Ref, <<"hij">>, <<"789">>, []),
[{<<"abc">>, <<"123">>},
{<<"def">>, <<"456">>},
{<<"hij">>, <<"789">>}] = lists:reverse(rocksdb:fold(Ref,
fun({K, V}, Acc) ->
[{K, V} | Acc]
end,
[], [])).
```

### Snapshot support

```
{ok, Db} = rocksdb:open("test.db", [{create_if_missing, true}]),
try
rocksdb:put(Db, <<"a">>, <<"x">>, []),
?assertEqual({ok, <<"x">>}, rocksdb:get(Db, <<"a">>, [])),
{ok, Snapshot} = rocksdb:snapshot(Db),
rocksdb:put(Db, <<"a">>, <<"y">>, []),
?assertEqual({ok, <<"y">>}, rocksdb:get(Db, <<"a">>, [])),
?assertEqual({ok, <<"x">>}, rocksdb:get(Db, <<"a">>, [{snapshot, Snapshot}])),
rocksdb:release_snapshot(Snapshot)
after
rocksdb:close(Db)
  end.
```

### Column families support

basic key/value operations

```
ColumnFamilies = [{"default", []}],
{ok, Db, [DefaultH]} = rocksdb:open_with_cf("test.db", [{create_if_missing, true}], ColumnFamilies),
ok = rocksdb:put(Db, DefaultH, <<"a">>, <<"a1">>, []),
{ok,  <<"a1">>} = rocksdb:get(Db, DefaultH, <<"a">>, []),
ok = rocksdb:put(Db, DefaultH, <<"b">>, <<"b1">>, []),
{ok, <<"b1">>} = rocksdb:get(Db, DefaultH, <<"b">>, []),
?assertEqual(2, rocksdb:count(Db,DefaultH)),

ok = rocksdb:delete(Db, DefaultH, <<"b">>, []),
not_found = rocksdb:get(Db, DefaultH, <<"b">>, []),
?assertEqual(1, rocksdb:count(Db, DefaultH)),

{ok, TestH} = rocksdb:create_column_family(Db, "test", []),
rocksdb:put(Db, TestH, <<"a">>, <<"a2">>, []),
{ok,  <<"a1">>} = rocksdb:get(Db, DefaultH, <<"a">>, []),
{ok,  <<"a2">>} = rocksdb:get(Db, TestH, <<"a">>, []),
?assertEqual(1, rocksdb:count(Db, TestH)),
rocksdb:close(Db)
```

iterator operations

```
{ok, Ref, [DefaultH]} = rocksdb:open_with_cf("ltest", [{create_if_missing, true}], [{"default", []}]),
{ok, TestH} = rocksdb:create_column_family(Ref, "test", []),
try
rocksdb:put(Ref, DefaultH, <<"a">>, <<"x">>, []),
rocksdb:put(Ref, DefaultH, <<"b">>, <<"y">>, []),
rocksdb:put(Ref, TestH, <<"a">>, <<"x1">>, []),
rocksdb:put(Ref, TestH, <<"b">>, <<"y1">>, []),

{ok, DefaultIt} = rocksdb:iterator(Ref, DefaultH, []),
{ok, TestIt} = rocksdb:iterator(Ref, TestH, []),

?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(DefaultIt, <<>>)),
?assertEqual({ok, <<"a">>, <<"x1">>},rocksdb:iterator_move(TestIt, <<>>)),
?assertEqual({ok, <<"b">>, <<"y">>},rocksdb:iterator_move(DefaultIt, next)),
?assertEqual({ok, <<"b">>, <<"y1">>},rocksdb:iterator_move(TestIt, next)),
?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(DefaultIt, prev)),
?assertEqual({ok, <<"a">>, <<"x1">>},rocksdb:iterator_move(TestIt, prev)),
ok = rocksdb:iterator_close(TestIt),
ok = rocksdb:iterator_close(DefaultIt)
after
rocksdb:close(Ref)
end.
```

### Batch support

```
Db = destroy_reopen("test.db", [{create_if_missing, true}]),

  {ok, Batch} = rocksdb:batch(),

ok = rocksdb:batch_put(Batch, <<"a">>, <<"v1">>),
ok = rocksdb:batch_put(Batch, <<"b">>, <<"v2">>),
ok = rocksdb:batch_delete(Batch, <<"b">>),
?assertEqual(3, rocksdb:batch_count(Batch)),

?assertEqual(not_found, rocksdb:get(Db, <<"a">>, [])),
?assertEqual(not_found, rocksdb:get(Db, <<"b">>, [])),

ok = rocksdb:write_batch(Db, Batch, []),

?assertEqual({ok, <<"v1">>}, rocksdb:get(Db, <<"a">>, [])),
?assertEqual(not_found, rocksdb:get(Db, <<"b">>, [])),

ok = rocksdb:close_batch(Batch),

close_destroy(Db, "test.db")
```

example of rollback support

```
{ok, Batch} = rocksdb:batch(),
ok = rocksdb:batch_put(Batch, <<"a">>, <<"v1">>),
ok = rocksdb:batch_put(Batch, <<"b">>, <<"v2">>),
ok = rocksdb:batch_savepoint(Batch),
ok = rocksdb:batch_put(Batch, <<"c">>, <<"v3">>),
?assertEqual(3, rocksdb:batch_count(Batch)),
?assertEqual([{put, <<"a">>, <<"v1">>},
            {put, <<"b">>, <<"v2">>},
            {put, <<"c">>, <<"v3">>}], rocksdb:batch_tolist(Batch)),
ok = rocksdb:batch_rollback(Batch),
?assertEqual(2, rocksdb:batch_count(Batch)),
?assertEqual([{put, <<"a">>, <<"v1">>},
            {put, <<"b">>, <<"v2">>}], rocksdb:batch_tolist(Batch)),
ok = rocksdb:close_batch(Batch)
```


## Modules ##


<table width="100%" border="0" summary="list of modules">
<tr><td><a href="http://gitlab.com/barrel-db/erlang-rocksdb/blob/master/doc/rocksdb.md" class="module">rocksdb</a></td></tr>
<tr><td><a href="http://gitlab.com/barrel-db/erlang-rocksdb/blob/master/doc/rocksdb_bump.md" class="module">rocksdb_bump</a></td></tr></table>

