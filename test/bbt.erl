-module(bbt).
-include_lib("eunit/include/eunit.hrl").

-define(rm_rf(Dir), rocksdb_test_util:rm_rf(Dir)).

no_bbt_test() ->
  DbName = "erocksdb.bbt.default",
  ?rm_rf(DbName),
  {ok, Db} = rocksdb:open(
    DbName,
    [
        {create_if_missing, true}
    ]
  ),
  %% Check LOG file to see if the compaction_style is not FIFO
  {ok, Log0} = file:read_file(lists:concat([DbName, "/LOG"])),
  ?assertMatch({match, _}, re:run(Log0, "partition_filters: 0")),
  ?assertMatch({match, _}, re:run(Log0, "index_type: 0")),

  ok = rocksdb:close(Db),
  ?rm_rf(DbName).

bbt_binary_search_test() ->
  DbName = "erocksdb.bbt_binary_search.default",
  ?rm_rf(DbName),
  {ok, Db} = rocksdb:open(
    DbName,
    [
        {create_if_missing, true},
        {block_based_table_options, [
            {partition_filters, true},
            {index_type, binary_search}
        ]}
    ]
  ),
  %% Check LOG file to see if the compaction_style is not FIFO
  {ok, Log0} = file:read_file(lists:concat([DbName, "/LOG"])),
  ?assertMatch({match, _}, re:run(Log0, "partition_filters: 1")),
  ?assertMatch({match, _}, re:run(Log0, "index_type: 0")),

  ok = rocksdb:close(Db),
  ?rm_rf(DbName).
