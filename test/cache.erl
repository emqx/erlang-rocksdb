-module(cache).


-include_lib("eunit/include/eunit.hrl").

cache_info_test() ->
  {ok, CHandle} = rocksdb:new_cache(lru, 101400),
  ok = rocksdb:set_strict_capacity_limit(CHandle, true),
  [{capacity,101400},
   {strict_capacity,true},
   {usage,0},
   {pinned_usage,0}] = rocksdb:cache_info(CHandle).
