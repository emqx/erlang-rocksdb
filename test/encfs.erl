%% Copyright (c) 2016-2020 Benoît Chesneau, 2025 Iliya Averianov.
%%
%% This file is provided to you under the Apache License,
%% Version 2.0 (the "License"); you may not use this file
%% except in compliance with the License.  You may obtain
%% a copy of the License at
%%
%%   http://www.apache.org/licenses/LICENSE-2.0
%%
%% Unless required by applicable law or agreed to in writing,
%% software distributed under the License is distributed on an
%% "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
%% KIND, either express or implied.  See the License for the
%% specific language governing permissions and limitations
%% under the License.
-module(encfs).

-compile([export_all/1]).
-include_lib("eunit/include/eunit.hrl").

basic_test() ->
  {ok, Env} = rocksdb:new_env({fs_uri, "provider={method=AES256CTR;id=AES;hex_instance_key=0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF};id=EncryptedFileSystem"}),
  Options = [{env, Env}, {create_if_missing, true}],
  {ok, Db} = rocksdb:open("erocksdb.encfs.test", Options),
  ok = rocksdb:put(Db, <<"a">>, <<"1">>, []),
  ?assertEqual({ok, <<"1">>}, rocksdb:get(Db, <<"a">>, [])),
  {ok, Db1} = rocksdb:open("erocksdb.encfs.test1", Options),
  ok = rocksdb:put(Db1, <<"a">>, <<"2">>, []),
  ?assertEqual({ok, <<"1">>}, rocksdb:get(Db, <<"a">>, [])),
  ?assertEqual({ok, <<"2">>}, rocksdb:get(Db1, <<"a">>, [])),
  ok = rocksdb:close(Db),
  ok = rocksdb:close(Db1),
  ok = destroy_and_rm("erocksdb.encfs.test", []),
  ok = destroy_and_rm("erocksdb.encfs.test1", []),
  ok.

destroy_and_rm(Dir, Options) ->
  rocksdb:destroy(Dir, Options),
  rocksdb_test_util:rm_rf(Dir).
