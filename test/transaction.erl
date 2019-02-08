-module(transaction).

-compile([export_all/1]).
-include_lib("eunit/include/eunit.hrl").
-include_lib("stdlib/include/assert.hrl").

destroy_reopen(DbName, Options) ->
    _ = rocksdb:destroy(DbName, []),
    {ok, Db, _} = rocksdb:open_optimistic_transaction_db(DbName, Options, [{"default", []}]),
    Db.

close_destroy(Db, DbName) ->
    rocksdb:close(Db),
    rocksdb:destroy(DbName, []).

basic_test() ->
    Db = destroy_reopen("test.db", [{create_if_missing, true}]),

    {ok, Transaction} = rocksdb:transaction(Db, []),

    ok = rocksdb:transaction_put(Transaction, <<"a">>, <<"v1">>),
    ok = rocksdb:transaction_put(Transaction, <<"b">>, <<"v2">>),

    ?assertEqual(not_found, rocksdb:get(Db, <<"a">>, [])),
    ?assertEqual(not_found, rocksdb:get(Db, <<"b">>, [])),

    ?assertEqual({ok, <<"v1">>}, rocksdb:transaction_get(Transaction, <<"a">>, [])),
    ?assertEqual({ok, <<"v2">>}, rocksdb:transaction_get(Transaction, <<"b">>, [])),

    ok = rocksdb:transaction_commit(Transaction),

    ?assertEqual({ok, <<"v1">>}, rocksdb:get(Db, <<"a">>, [])),
    ?assertEqual({ok, <<"v2">>}, rocksdb:get(Db, <<"b">>, [])),

    ?assertException(error, badarg, rocksdb:transaction_put(Transaction, <<"a">>, <<"v2">>)),
    close_destroy(Db, "test.db"),
    ok.

delete_test() ->
    Db = destroy_reopen("test.db", [{create_if_missing, true}]),

    {ok, Transaction} = rocksdb:transaction(Db, []),

    ok = rocksdb:transaction_put(Transaction, <<"a">>, <<"v1">>),
    ok = rocksdb:transaction_put(Transaction, <<"b">>, <<"v2">>),
    ok = rocksdb:transaction_delete(Transaction, <<"b">>),
    ok = rocksdb:transaction_delete(Transaction, <<"c">>),

    ?assertEqual(not_found, rocksdb:get(Db, <<"a">>, [])),
    ?assertEqual(not_found, rocksdb:get(Db, <<"b">>, [])),

    ?assertEqual({ok, <<"v1">>}, rocksdb:transaction_get(Transaction, <<"a">>, [])),
    ?assertEqual(not_found, rocksdb:transaction_get(Transaction, <<"b">>, [])),

    ok = rocksdb:transaction_commit(Transaction),

    ?assertEqual({ok, <<"v1">>}, rocksdb:get(Db, <<"a">>, [])),
    ?assertEqual(not_found, rocksdb:get(Db, <<"b">>, [])),

    close_destroy(Db, "test.db"),
    ok.

%% not porting these to transactions because we don't need them currently.
%% rollback_test() ->
%%   {ok, Batch} = rocksdb:batch(),
%%   ok = rocksdb:batch_put(Batch, <<"a">>, <<"v1">>),
%%   ok = rocksdb:batch_put(Batch, <<"b">>, <<"v2">>),
%%   ok = rocksdb:batch_savepoint(Batch),
%%   ok = rocksdb:batch_put(Batch, <<"c">>, <<"v3">>),
%%   ?assertEqual(3, rocksdb:batch_count(Batch)),
%%   ?assertEqual([{put, <<"a">>, <<"v1">>},
%%                 {put, <<"b">>, <<"v2">>},
%%                 {put, <<"c">>, <<"v3">>}], rocksdb:batch_tolist(Batch)),
%%   ok = rocksdb:batch_rollback(Batch),
%%   ?assertEqual(2, rocksdb:batch_count(Batch)),
%%   ?assertEqual([{put, <<"a">>, <<"v1">>},
%%                 {put, <<"b">>, <<"v2">>}], rocksdb:batch_tolist(Batch)),
%%   ok = rocksdb:release_batch(Batch).


%% rollback_over_savepoint_test() ->
%%   {ok, Batch} = rocksdb:batch(),
%%   ok = rocksdb:batch_put(Batch, <<"a">>, <<"v1">>),
%%   ok = rocksdb:batch_put(Batch, <<"b">>, <<"v2">>),
%%   ok = rocksdb:batch_savepoint(Batch),
%%   ok = rocksdb:batch_put(Batch, <<"c">>, <<"v3">>),
%%   ?assertEqual(3, rocksdb:batch_count(Batch)),
%%   ?assertEqual([{put, <<"a">>, <<"v1">>},
%%                 {put, <<"b">>, <<"v2">>},
%%                 {put, <<"c">>, <<"v3">>}], rocksdb:batch_tolist(Batch)),
%%   ok = rocksdb:batch_rollback(Batch),
%%   ?assertEqual(2, rocksdb:batch_count(Batch)),
%%   ?assertEqual([{put, <<"a">>, <<"v1">>},
%%                 {put, <<"b">>, <<"v2">>}], rocksdb:batch_tolist(Batch)),

%%   ?assertMatch({error, _}, rocksdb:batch_rollback(Batch)),

%%   ok = rocksdb:release_batch(Batch).


%% currently not supported properly on the rocks side, see comment in c_src/transaction.cc
%% merge_test() ->
%%     Db = destroy_reopen("test.db", [{create_if_missing, true}, {merge_operator, erlang_merge_operator}]),

%%     ok = rocksdb:put(Db, <<"i">>, term_to_binary(0), []),
%%     {ok, IBin0} = rocksdb:get(Db, <<"i">>, []),
%%     0 = binary_to_term(IBin0),

%%     {ok, Transaction} = rocksdb:transaction(Db, []),

%%     ok = rocksdb:transaction_merge(Transaction, <<"i">>, term_to_binary({int_add, 1})),
%%     {ok, IBin0} = rocksdb:get(Db, <<"i">>, []),
%%     0 = binary_to_term(IBin0),
%%     %% {ok, ValBinA} = rocksdb:transaction_get(Transaction, <<"i">>, []),
%%     %% ?assertEqual(1, binary_to_term(ValBinA)),

%%     ok = rocksdb:transaction_merge(Transaction, <<"i">>, term_to_binary({int_add, 2})),
%%     {ok, IBin0} = rocksdb:get(Db, <<"i">>, []),
%%     0 = binary_to_term(IBin0),
%%     %% {ok, ValBinB} = rocksdb:transaction_get(Transaction, <<"i">>, []),
%%     %% ?assertEqual(3, binary_to_term(ValBinB)),

%%     ok = rocksdb:transaction_merge(Transaction, <<"i">>, term_to_binary({int_add, -1})),
%%     {ok, IBin0} = rocksdb:get(Db, <<"i">>, []),
%%     0 = binary_to_term(IBin0),
%%     %% {ok, ValBinC} = rocksdb:transaction_get(Transaction, <<"i">>, []),
%%     %% ?assertEqual(2, binary_to_term(ValBinC)),

%%     ok = rocksdb:transaction_commit(Transaction),
%%     {ok, IBin1} = rocksdb:get(Db, <<"i">>, []),
%%     2 = binary_to_term(IBin1),

%%     close_destroy(Db, "test.db").
