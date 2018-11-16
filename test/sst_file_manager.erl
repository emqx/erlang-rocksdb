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
-module(sst_file_manager).

-include_lib("eunit/include/eunit.hrl").



basic_test() ->
    {ok, Env} = rocksdb:default_env(),
    {ok, Mgr} = rocksdb:new_sst_file_manager(Env),
    0.25 = rocksdb_sst_file_manager:get_max_trash_db_ratio(Mgr),
    0 = rocksdb_sst_file_manager:get_delete_rate_bytes_per_second(Mgr),
    ok = rocksdb_sst_file_manager:set_delete_rate_bytes_per_second(Mgr, 1024),
    1024 = rocksdb_sst_file_manager:get_delete_rate_bytes_per_second(Mgr),
    ok = rocksdb:release_sst_file_manager(Mgr).

simple_options_test() ->
  {ok, Env} = rocksdb:default_env(),
  {ok, Mgr} = rocksdb:new_sst_file_manager(Env),
  Options = [{create_if_missing, true}, {env, Env}, {sst_file_manager, Mgr}],

  {ok, Ref} = rocksdb:open("/tmp/rocksdb_sst_file_mgr.test", Options),
  Key = <<"key">>,
  OriginalData = <<"sample_data">>,
  ok = rocksdb:put(Ref, Key, OriginalData, []),
  {ok, Data} = rocksdb:get(Ref, Key, []),
  ?assertEqual(Data, OriginalData),
  ok = rocksdb:close(Ref),
  ok = rocksdb:release_sst_file_manager(Mgr),
  ok = rocksdb:destroy_env(Env),
  ok = rocksdb:destroy("/tmp/rocksdb_sst_file_mgr.test", []),
  ok.
