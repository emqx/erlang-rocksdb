%% Copyright (c) 2018 Benoit Chesneau
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
%%
%% -------------------------------------------------------------------

-module(merge).

-include_lib("eunit/include/eunit.hrl").


merge_int_test() ->
  [] = os:cmd("rm -rf /tmp/rocksdb_merge_db.test"),
  {ok, Db} = rocksdb:open("/tmp/rocksdb_merge_db.test",
                           [{create_if_missing, true},
                            {merge_operator, erlang_merge_operator}]),

  ok = rocksdb:put(Db, <<"i">>, term_to_binary(0), []),
  {ok, IBin0} = rocksdb:get(Db, <<"i">>, []),
  0 = binary_to_term(IBin0),

  ok = rocksdb:merge(Db, <<"i">>, term_to_binary({int_add, 1}), []),
  {ok, IBin1} = rocksdb:get(Db, <<"i">>, []),
  1 = binary_to_term(IBin1),

  ok = rocksdb:merge(Db, <<"i">>, term_to_binary({int_add, 2}), []),
  {ok, IBin2} = rocksdb:get(Db, <<"i">>, []),
  3 = binary_to_term(IBin2),

  ok = rocksdb:merge(Db, <<"i">>, term_to_binary({int_add, -1}), []),
  {ok, IBin3} = rocksdb:get(Db, <<"i">>, []),
  2 = binary_to_term(IBin3),

  ok = rocksdb:put(Db, <<"i">>, term_to_binary(0), []),
  {ok, IBin4} = rocksdb:get(Db, <<"i">>, []),
  0 = binary_to_term(IBin4),


  ok = rocksdb:close(Db),
  ok = rocksdb:destroy("/tmp/rocksdb_merge_db.test", []).






