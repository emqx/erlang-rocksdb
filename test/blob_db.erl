-module(blob_db).

-export([basic_test/0
        ,cache_test/0]).
-define(rm_rf(Dir), rocksdb_test_util:rm_rf(Dir)).


basic_test() ->
  ?rm_rf("test_blobdb"),
  {ok, Db} = 
    rocksdb:open(
      "test_blobdb", 
      [{create_if_missing, true}
      ,{enable_blob_files, true}
      ,{min_blob_size, 0}
      ,{blob_garbage_collection_age_cutoff, 0.25}]
    ),
  try
    ok = rocksdb:put(Db, <<"key">>, <<"blob_value">>, []),
    ok = rocksdb:flush(Db, []),
    {ok, <<"blob_value">>} = rocksdb:get(Db, <<"key">>, [])
  after
    ok = rocksdb:close(Db),
    ?rm_rf("test_blobdb")
  end,
  ok.

cache_test() ->
  ?rm_rf("test_cacheblobdb"),
  {ok, CHandle} = rocksdb:new_cache(lru, 2097152),
  {ok, Db} = 
    rocksdb:open(
      "test_cacheblobdb", 
      [{create_if_missing, true}
      ,{enable_blob_files, true}
      ,{blob_cache, CHandle}]
    ),
  try
    ok = rocksdb:put(Db, <<"key">>, <<"blob_value">>, []),
    ok = rocksdb:flush(Db, []),
    {ok, <<"blob_value">>} = rocksdb:get(Db, <<"key">>, [])
  after
    ok = rocksdb:close(Db),
    ?rm_rf("test_blobdb")
  end,
  ok.




  


