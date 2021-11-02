-module(statistics).


-include_lib("eunit/include/eunit.hrl").

statistics_test() ->
  {ok, SHandle} = rocksdb:new_statistics(),
  ok = rocksdb:set_stats_level(SHandle, stats_all),
  [{stats_level, stats_all}] = rocksdb:statistics_info(SHandle),
  ok = rocksdb:set_stats_level(SHandle, stats_except_histogram_or_timers),
  [{stats_level, stats_except_histogram_or_timers}] = rocksdb:statistics_info(SHandle),
  ok = rocksdb:set_stats_level(SHandle, stats_except_tickers),
  [{stats_level, stats_disable_all}] = rocksdb:statistics_info(SHandle),
  ok = rocksdb:release_statistics(SHandle).

