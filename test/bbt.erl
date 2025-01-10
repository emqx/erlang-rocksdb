-module(bbt).
-include_lib("eunit/include/eunit.hrl").

-define(rm_rf(Dir), rocksdb_test_util:rm_rf(Dir)).

bbt_partion_filters_default_test() ->
  DbName = "erocksdb.bbt.partition_filters.default",
  ?rm_rf(DbName),
  {ok, Db} = rocksdb:open(
    DbName,
    [
        {create_if_missing, true}
    ]
  ),
  %% Check LOG file for check partition_filters set to 0
  {ok, Log0} = file:read_file(lists:concat([DbName, "/LOG"])),
  ?assertMatch({match, _}, re:run(Log0, "partition_filters: 0")),

  ok = rocksdb:close(Db),
  ?rm_rf(DbName).

bbt_partion_filters_false_test() ->
  DbName = "erocksdb.bbt.partition_filters.false",
  ?rm_rf(DbName),
  {ok, Db} = rocksdb:open(
    DbName,
    [
        {create_if_missing, true}
    ]
  ),
  %% Check LOG file for check partition_filters set to 0
  {ok, Log0} = file:read_file(lists:concat([DbName, "/LOG"])),
  ?assertMatch({match, _}, re:run(Log0, "partition_filters: 0")),
  ?assertMatch({match, _}, re:run(Log0, "index_type: 0")),

  ok = rocksdb:close(Db),
  ?rm_rf(DbName).


bbt_partion_filters_true_test() ->
  DbName = "erocksdb.bbt.partition_filters.true",
  ?rm_rf(DbName),
  {ok, Db} = rocksdb:open(
    DbName,
    [
        {create_if_missing, true},
        {block_based_table_options, [
            {partition_filters, true}
        ]}
    ]
  ),
  %% Check LOG file for check partition_filters set to 0
  {ok, Log0} = file:read_file(lists:concat([DbName, "/LOG"])),
  ?assertMatch({match, _}, re:run(Log0, "partition_filters: 1")),
  ?assertMatch({match, _}, re:run(Log0, "index_type: 2")),
  ok = rocksdb:close(Db),
  ?rm_rf(DbName).
