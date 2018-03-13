-module(batch).

-compile([export_all/1]).
-include_lib("eunit/include/eunit.hrl").

destroy_reopen(DbName, Options) ->
  _ = rocksdb:destroy(DbName, []),
  {ok, Db} = rocksdb:open(DbName, Options),
  Db.

close_destroy(Db, DbName) ->
  rocksdb:close(Db),
  rocksdb:destroy(DbName, []).

basic_test() ->
  Db = destroy_reopen("test.db", [{create_if_missing, true}]),

  {ok, Batch} = rocksdb:batch(),

  ok = rocksdb:batch_put(Batch, <<"a">>, <<"v1">>),
  ok = rocksdb:batch_put(Batch, <<"b">>, <<"v2">>),
  ?assertEqual(2, rocksdb:batch_count(Batch)),

  ?assertEqual(not_found, rocksdb:get(Db, <<"a">>, [])),
  ?assertEqual(not_found, rocksdb:get(Db, <<"b">>, [])),

  ok = rocksdb:write_batch(Db, Batch, []),

  ?assertEqual({ok, <<"v1">>}, rocksdb:get(Db, <<"a">>, [])),
  ?assertEqual({ok, <<"v2">>}, rocksdb:get(Db, <<"b">>, [])),

  ok = rocksdb:close_batch(Batch),

  close_destroy(Db, "test.db"),
  ok.

delete_test() ->
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

  close_destroy(Db, "test.db"),
  ok.

single_delete_test() ->
  Db = destroy_reopen("test.db", [{create_if_missing, true}]),
  {ok, Batch} = rocksdb:batch(),
  ok = rocksdb:batch_put(Batch, <<"a">>, <<"v1">>),
  ok = rocksdb:write_batch(Db, Batch, []),
  ?assertEqual({ok, <<"v1">>}, rocksdb:get(Db, <<"a">>, [])),
  ok = rocksdb:close_batch(Batch),
  {ok, Batch1} = rocksdb:batch(),
  ok = rocksdb:batch_single_delete(Batch1, <<"a">>),
  ok = rocksdb:write_batch(Db, Batch1, []),
  ok = rocksdb:close_batch(Batch1),
  ?assertEqual(not_found, rocksdb:get(Db, <<"a">>, [])),
  close_destroy(Db, "test.db"),
  ok.

delete_with_notfound_test() ->
  Db = destroy_reopen("test.db", [{create_if_missing, true}]),

  {ok, Batch} = rocksdb:batch(),

  ok = rocksdb:batch_put(Batch, <<"a">>, <<"v1">>),
  ok = rocksdb:batch_put(Batch, <<"b">>, <<"v2">>),
  ok = rocksdb:batch_delete(Batch, <<"c">>),
  ?assertEqual(3, rocksdb:batch_count(Batch)),

  ?assertEqual(not_found, rocksdb:get(Db, <<"a">>, [])),
  ?assertEqual(not_found, rocksdb:get(Db, <<"b">>, [])),

  ok = rocksdb:write_batch(Db, Batch, []),

  ?assertEqual({ok, <<"v1">>}, rocksdb:get(Db, <<"a">>, [])),
  ?assertEqual({ok, <<"v2">>}, rocksdb:get(Db, <<"b">>, [])),

  ok = rocksdb:close_batch(Batch),

  close_destroy(Db, "test.db"),
  ok.

tolist_test() ->
  {ok, Batch} = rocksdb:batch(),
  ok = rocksdb:batch_put(Batch, <<"a">>, <<"v1">>),
  ok = rocksdb:batch_put(Batch, <<"b">>, <<"v2">>),
  ?assertEqual(2, rocksdb:batch_count(Batch)),
  ?assertEqual([{put, <<"a">>, <<"v1">>}, {put, <<"b">>, <<"v2">>}], rocksdb:batch_tolist(Batch)),
  ok = rocksdb:close_batch(Batch),
  ok.

rollback_test() ->
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
  ok = rocksdb:close_batch(Batch).


rollback_over_savepoint_test() ->
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

  ?assertMatch({error, _}, rocksdb:batch_rollback(Batch)),

  ok = rocksdb:close_batch(Batch).