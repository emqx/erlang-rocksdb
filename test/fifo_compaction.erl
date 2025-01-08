%% https://github.com/facebook/rocksdb/wiki/FIFO-compaction-style

-module(fifo_compaction).
-include_lib("eunit/include/eunit.hrl").

-define(rm_rf(Dir), rocksdb_test_util:rm_rf(Dir)).

no_fifo_compaction_test() ->
  DbName = "erocksdb.fifo.default",
  ?rm_rf(DbName),
  {ok, Db} = rocksdb:open(
    DbName,
    [
        {create_if_missing, true}
    ]
  ),
  %% Check LOG file to see if the compaction_style is not FIFO
  {ok, Log0} = file:read_file(lists:concat([DbName, "/LOG"])),
  ?assertMatch(nomatch, re:run(Log0, "Options.compaction_style: kCompactionStyleFIFO")),
  ok = rocksdb:close(Db),
  ?rm_rf(DbName).

fifo_compaction_with_ttl_4_test() ->
    test_with_ttl(4).

fifo_compaction_with_ttl_8_test() ->
    test_with_ttl(4).

fifo_compaction_options_allow_compaction_settings_1_test() ->
  DbName = "erocksdb.fifo.allow_compaction_settings_1",
  ?rm_rf(DbName),
  {ok, Db} = rocksdb:open(
    DbName,
    [
        {create_if_missing, true},
        {compaction_style, fifo},
        {compaction_options_fifo, [
            {allow_compaction, true},
            {age_for_warm, 10},
            {max_table_files_size, 20}
        ]}
    ]
  ),
  %% Check LOG file to see if the compaction_style is not FIFO
  {ok, Log0} = file:read_file(lists:concat([DbName, "/LOG"])),
  ?assertMatch({match, _}, re:run(Log0, "Options.compaction_style: kCompactionStyleFIFO")),
  ?assertMatch({match, _}, re:run(Log0, "Options.compaction_options_fifo.allow_compaction: 1")),
  ?assertMatch({match, _}, re:run(Log0, "Options.compaction_options_fifo.max_table_files_size: 20")),

  ok = rocksdb:close(Db),
  ?rm_rf(DbName).

fifo_compaction_options_allow_compaction_settings_2_test() ->
  DbName = "erocksdb.fifo.allow_compaction_settings_2",
  ?rm_rf(DbName),
  {ok, Db} = rocksdb:open(
    DbName,
    [
        {create_if_missing, true},
        {compaction_style, fifo},
        {compaction_options_fifo, [
            {allow_compaction, false},
            {age_for_warm, 40},
            {max_table_files_size, 50}
        ]}
    ]
  ),
  %% Check LOG file to see if the compaction_style is not FIFO
  {ok, Log0} = file:read_file(lists:concat([DbName, "/LOG"])),
  ?assertMatch({match, _}, re:run(Log0, "Options.compaction_style: kCompactionStyleFIFO")),
  ?assertMatch({match, _}, re:run(Log0, "Options.compaction_options_fifo.allow_compaction: 0")),
  ?assertMatch({match, _}, re:run(Log0, "Options.compaction_options_fifo.max_table_files_size: 50")),

  ok = rocksdb:close(Db),
  ?rm_rf(DbName).


test_with_ttl(TTL) ->
  TTLAsList = integer_to_list(TTL),
  DbName = lists:concat(["erocksdb.fifo",".",TTLAsList]),
  ?rm_rf(DbName),
  {ok, Db} = rocksdb:open(
    DbName,
    [
        {create_if_missing, true},
        {compaction_style, fifo},
        {ttl, TTL}
    ]
  ),
  %% Check LOG file to see if the compaction_style fifo and ttl was correctly passed
  {ok, Log0} = file:read_file(lists:concat([DbName, "/LOG"])),
  ?assertMatch({match, _}, re:run(Log0, "Options.compaction_style: kCompactionStyleFIFO")),
  ?assertMatch({match, _}, re:run(Log0, lists:concat(["Options.ttl: ",TTLAsList]))),
  ok = rocksdb:close(Db),
  ?rm_rf(DbName).

