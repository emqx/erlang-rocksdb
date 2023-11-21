-module(secondary).


-include_lib("eunit/include/eunit.hrl").


secondary_connection_test() ->
    Dir1 = "erocksdb.secondary1.test1",
    rocksdb_test_util:rm_rf(Dir1),
    Dir2 = "erocksdb.secondary2.test1",
    rocksdb_test_util:rm_rf(Dir2),
    {ok, Ref1} = rocksdb:open(Dir1, [{create_if_missing, true}]),
    ok = rocksdb:put(Ref1, <<"abc">>, <<"123">>, []),
    {ok, Ref2} = rocksdb:open_secondary(Dir1, Dir2, []),
    {ok, <<"123">>} = rocksdb:get(Ref2, <<"abc">>, []),
    ok = rocksdb:put(Ref1, <<"def">>, <<"456">>, []),
    not_found = rocksdb:get(Ref2, <<"def">>, []),
    ok = rocksdb:try_catch_up_with_primary(Ref2),
    {ok, <<"456">>} = rocksdb:get(Ref2, <<"def">>, []),
    ok = rocksdb:close(Ref2),
    ok = rocksdb:close(Ref1),
    rocksdb_test_util:rm_rf(Dir2),
    rocksdb_test_util:rm_rf(Dir1).


secondary_connection_with_cf_test() ->
    Dir1 = "erocksdb.secondary1.test2",
    rocksdb_test_util:rm_rf(Dir1),
    Dir2 = "erocksdb.secondary2.test2",
    rocksdb_test_util:rm_rf(Dir2),
    {ok, Ref1, [DefaultCF1, AnotherCF1]} = rocksdb:open(Dir1,
        [{create_if_missing, true}, {create_missing_column_families, true}],
        [{"default", []}, {"another", []}]),
    ok = rocksdb:put(Ref1, <<"abc">>, <<"123">>, []),
    {ok, Ref2, [DefaultCF2, AnotherCF2]} = rocksdb:open_secondary(Dir1, Dir2, [],
        [{"default", []}, {"another", []}]),
    {ok, <<"123">>} = rocksdb:get(Ref2, <<"abc">>, []),
    ok = rocksdb:put(Ref1, <<"def">>, <<"456">>, []),
    not_found = rocksdb:get(Ref2, <<"def">>, []),
    ok = rocksdb:try_catch_up_with_primary(Ref2),
    {ok, <<"456">>} = rocksdb:get(Ref2, <<"def">>, []),
    ok = rocksdb:close(Ref2),
    ok = rocksdb:close(Ref1),
    rocksdb_test_util:rm_rf(Dir2),
    rocksdb_test_util:rm_rf(Dir1).
