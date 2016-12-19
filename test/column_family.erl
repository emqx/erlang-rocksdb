-module(column_family).


-compile([export_all/1]).
-include_lib("eunit/include/eunit.hrl").


basic_test() ->
    rocksdb:destroy("test.db", []),
    ColumnFamilies = [{"default", []}],
    {ok, Db, Handles1} = rocksdb:open_with_cf("test.db", [{create_if_missing, true}], ColumnFamilies),
    ?assertEqual(1, length(Handles1)),
    ?assertEqual({ok, ["default"]}, rocksdb:list_column_families("test.db", [])),
    {ok, Handle} = rocksdb:create_column_family(Db, "test", []),
    ?assertEqual({ok, ["default", "test"]}, rocksdb:list_column_families("test.db", [])),
    ok = rocksdb:drop_column_family(Handle),
    ?assertEqual({ok, ["default"]}, rocksdb:list_column_families("test.db", [])),
    rocksdb:close(Db),
    
    ok.

column_order_test() ->
    rocksdb:destroy("test.db", []),
    ColumnFamilies = [{"default", []}],
    {ok, Db, Handles1} = rocksdb:open_with_cf("test.db", [{create_if_missing, true}], ColumnFamilies),
    ?assertEqual(1, length(Handles1)),
    ?assertEqual({ok, ["default"]}, rocksdb:list_column_families("test.db", [])),
    {ok, _Handle} = rocksdb:create_column_family(Db, "test", []),
    ok = rocksdb:close(Db),
    ?assertEqual({ok, ["default", "test"]}, rocksdb:list_column_families("test.db", [])),
    ColumnFamilies2 = [{"default", []}, {"test", []}],
    {ok, Db2, Handles2} = rocksdb:open_with_cf("test.db", [{create_if_missing, true}], ColumnFamilies2),
    [_DefaultH, TestH] = Handles2,
    ok = rocksdb:drop_column_family(TestH),
    ?assertEqual({ok, ["default"]}, rocksdb:list_column_families("test.db", [])),
    rocksdb:close(Db2),
    ok.

try_remove_default_test() ->
    rocksdb:destroy("test.db", []),
    ColumnFamilies = [{"default", []}],
    {ok, Db, [DefaultH]} = rocksdb:open_with_cf("test.db", [{create_if_missing, true}], ColumnFamilies),
    {error, _} = rocksdb:drop_column_family(DefaultH),
    {ok, _Handle} = rocksdb:create_column_family(Db, "test", []),
    rocksdb:close(Db),
    ColumnFamilies2 = [{"default", []}, {"test", []}],
    {ok, Db2, [DefaultH2, _]} = rocksdb:open_with_cf("test.db", [{create_if_missing, true}], ColumnFamilies2),
    {error, _} = rocksdb:drop_column_family(DefaultH2),
    rocksdb:close(Db2),
    ok.

basic_kvs_test() ->
    rocksdb:destroy("test.db", []),
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
    rocksdb:close(Db),
    ok.

iterators_test() ->
    os:cmd("rm -rf ltest"),  % NOTE
    {ok, Ref, [DefaultH]} = rocksdb:open_with_cf("ltest", [{create_if_missing, true}], [{"default", []}]),
    {ok, TestH} = rocksdb:create_column_family(Ref, "test", []),
    try
        rocksdb:put(Ref, DefaultH, <<"a">>, <<"x">>, []),
        rocksdb:put(Ref, DefaultH, <<"b">>, <<"y">>, []),
        rocksdb:put(Ref, TestH, <<"a">>, <<"x1">>, []),
        rocksdb:put(Ref, TestH, <<"b">>, <<"y1">>, []),

        {ok, [DefaultIt, TestIt]} = rocksdb:iterators(Ref, [DefaultH, TestH], []),
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