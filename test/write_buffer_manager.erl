%%% -*- erlang -*-
%%
%% Copyright (c) 2018 Benoit Chesneau
%%
%% Licensed under the Apache License, Version 2.0 (the "License");
%% you may not use this file except in compliance with the License.
%% You may obtain a copy of the License at
%%
%% http://www.apache.org/licenses/LICENSE-2.0
%%
%% Unless required by applicable law or agreed to in writing, software
%% distributed under the License is distributed on an "AS IS" BASIS,
%% WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
%% See the License for the specific language governing permissions and
%% limitations under the License.
-module(write_buffer_manager).

-include_lib("eunit/include/eunit.hrl").


basic_test() ->
    Sz = 64 bsl 20,
    {ok, Cache} = rocksdb:new_lru_cache(1024*1024*128),
    {ok, Mgr} = rocksdb:new_write_buffer_manager(Sz, Cache),
    Sz = rocksdb_write_buffer_manager:buffer_size(Mgr),
    true = rocksdb_write_buffer_manager:enabled(Mgr),
    0 = rocksdb_write_buffer_manager:memory_usage(Mgr),
    rocksdb:release_write_buffer_manager(Mgr).


simple_options_test() ->
  Sz = 64 bsl 20,
  {ok, Mgr} = rocksdb:new_write_buffer_manager(Sz),
  Options = [{create_if_missing, true}, {write_buffer_manager, Mgr}],
  {ok, Ref} = rocksdb:open("/tmp/rocksdb_write_buffer_mgr.test", Options),
  {ok, Ref2} = rocksdb:open("/tmp/rocksdb_write_buffer_mgr2.test", Options),
  Key = <<"key">>,
  OriginalData = <<"sample_data">>,
  ok = rocksdb:put(Ref, Key, OriginalData, []),
  ok = rocksdb:put(Ref2, Key, OriginalData, []),
  {ok, Data} = rocksdb:get(Ref, Key, []),
  {ok, Data} = rocksdb:get(Ref2, Key, []),
  ?assertEqual(Data, OriginalData),
  ok = rocksdb:close(Ref),
  ok = rocksdb:close(Ref2),
  ok = rocksdb:release_write_buffer_manager(Mgr),
  ok = rocksdb:destroy("/tmp/rocksdb_write_buffer_mgr.test", []),
  ok = rocksdb:destroy("/tmp/rocksdb_write_buffer_mgr2.test", []),
  ok.
