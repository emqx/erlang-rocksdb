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

merge_list_append_test() ->
  [] = os:cmd("rm -rf /tmp/rocksdb_merge_db.test"),
  {ok, Db} = rocksdb:open("/tmp/rocksdb_merge_db.test",
                           [{create_if_missing, true},
                            {merge_operator, erlang_merge_operator}]),

  ok = rocksdb:put(Db, <<"list">>, term_to_binary([a, b]), []),
  {ok, Bin0} = rocksdb:get(Db, <<"list">>, []),
  [a, b] = binary_to_term(Bin0),

  ok = rocksdb:merge(Db, <<"list">>, term_to_binary({list_append, [c, d]}), []),
  {ok, Bin1} = rocksdb:get(Db, <<"list">>, []),
  [a, b, c, d] = binary_to_term(Bin1),

  ok = rocksdb:merge(Db, <<"list">>, term_to_binary({list_append, [d, e]}), []),
  {ok, Bin2} = rocksdb:get(Db, <<"list">>, []),
  [a, b, c, d, d, e] = binary_to_term(Bin2),

  ok = rocksdb:close(Db),
  ok = rocksdb:destroy("/tmp/rocksdb_merge_db.test", []).

merge_list_substract_test() ->
  [] = os:cmd("rm -rf /tmp/rocksdb_merge_db.test"),
  {ok, Db} = rocksdb:open("/tmp/rocksdb_merge_db.test",
                           [{create_if_missing, true},
                            {merge_operator, erlang_merge_operator}]),

  ok = rocksdb:put(Db, <<"list">>, term_to_binary([a, b, c, d, e]), []),
  {ok, Bin0} = rocksdb:get(Db, <<"list">>, []),
  [a, b, c, d, e] = binary_to_term(Bin0),

  ok = rocksdb:merge(Db, <<"list">>, term_to_binary({list_substract, [c, a]}), []),
  {ok, Bin1} = rocksdb:get(Db, <<"list">>, []),
  [b, d, e] = binary_to_term(Bin1),

  ok = rocksdb:close(Db),
  ok = rocksdb:destroy("/tmp/rocksdb_merge_db.test", []).



merge_list_set_test() ->
  [] = os:cmd("rm -rf /tmp/rocksdb_merge_db.test"),
  {ok, Db} = rocksdb:open("/tmp/rocksdb_merge_db.test",
                           [{create_if_missing, true},
                            {merge_operator, erlang_merge_operator}]),

  ok = rocksdb:put(Db, <<"list">>, term_to_binary([a, b, c, d, e]), []),
  {ok, Bin0} = rocksdb:get(Db, <<"list">>, []),
  [a, b, c, d, e] = binary_to_term(Bin0),

  ok = rocksdb:merge(Db, <<"list">>, term_to_binary({list_set, 2, 'c1'}), []),
  {ok, Bin1} = rocksdb:get(Db, <<"list">>, []),
  [a, b, 'c1', d, e] = binary_to_term(Bin1),

  ok = rocksdb:merge(Db, <<"list">>, term_to_binary({list_set, 4, 'e1'}), []),
  {ok, Bin2} = rocksdb:get(Db, <<"list">>, []),
  [a, b, 'c1', d, 'e1'] = binary_to_term(Bin2),

  ok = rocksdb:merge(Db, <<"list">>, term_to_binary({list_set, 5, error}), []),
  corruption = rocksdb:get(Db, <<"list">>, []),

  ok = rocksdb:close(Db),
  ok = rocksdb:destroy("/tmp/rocksdb_merge_db.test", []).

merge_list_delete_test() ->
  [] = os:cmd("rm -rf /tmp/rocksdb_merge_db.test"),
  {ok, Db} = rocksdb:open("/tmp/rocksdb_merge_db.test",
                           [{create_if_missing, true},
                            {merge_operator, erlang_merge_operator}]),

  ok = rocksdb:put(Db, <<"list">>, term_to_binary([a, b, c, d, e, f, g]), []),
  {ok, Bin0} = rocksdb:get(Db, <<"list">>, []),
  [a, b, c, d, e, f, g] = binary_to_term(Bin0),

  ok = rocksdb:merge(Db, <<"list">>, term_to_binary({list_delete, 2}), []),
  {ok, Bin1} = rocksdb:get(Db, <<"list">>, []),
  [a, b, d, e, f, g] = binary_to_term(Bin1),

  ok = rocksdb:merge(Db, <<"list">>, term_to_binary({list_delete, 2, 4}), []),
  {ok, Bin2} = rocksdb:get(Db, <<"list">>, []),
  [a, b, g] = binary_to_term(Bin2),

  ok = rocksdb:merge(Db, <<"list">>, term_to_binary({list_delete, 6}), []),
  corruption = rocksdb:get(Db, <<"list">>, []),

  ok = rocksdb:close(Db),
  ok = rocksdb:destroy("/tmp/rocksdb_merge_db.test", []).

merge_list_insert_test() ->
  [] = os:cmd("rm -rf /tmp/rocksdb_merge_db.test"),
  {ok, Db} = rocksdb:open("/tmp/rocksdb_merge_db.test",
                           [{create_if_missing, true},
                            {merge_operator, erlang_merge_operator}]),

  ok = rocksdb:put(Db, <<"list">>, term_to_binary([a, b, c, d, e, f, g]), []),
  {ok, Bin0} = rocksdb:get(Db, <<"list">>, []),
  [a, b, c, d, e, f, g] = binary_to_term(Bin0),

  ok = rocksdb:merge(Db, <<"list">>, term_to_binary({list_insert, 2, [h, i]}), []),
  {ok, Bin1} = rocksdb:get(Db, <<"list">>, []),
  [a, b, h, i, c, d, e, f, g] = binary_to_term(Bin1),

  ok = rocksdb:merge(Db, <<"list">>, term_to_binary({list_insert, 9, [j]}), []),
  corruption = rocksdb:get(Db, <<"list">>, []),

  ok = rocksdb:close(Db),
  ok = rocksdb:destroy("/tmp/rocksdb_merge_db.test", []).

merge_binary_append_test() ->
  [] = os:cmd("rm -rf /tmp/rocksdb_merge_db.test"),
  {ok, Db} = rocksdb:open("/tmp/rocksdb_merge_db.test",
                           [{create_if_missing, true},
                            {merge_operator, erlang_merge_operator}]),

  ok = rocksdb:put(Db, <<"bin">>, <<"test">>, []),
  {ok, <<"test">>} = rocksdb:get(Db, <<"bin">>, []),

  ok = rocksdb:merge(Db, <<"bin">>, term_to_binary({binary_append, <<"abc">>}), []),
  {ok, <<"testabc">>} = rocksdb:get(Db, <<"bin">>, []),

  ok = rocksdb:merge(Db, <<"bin">>, term_to_binary({binary_append, <<"de">>}), []),
  {ok, <<"testabcde">>} = rocksdb:get(Db, <<"bin">>, []),


  ok = rocksdb:put(Db, <<"encbin">>, term_to_binary(<<"test">>), []),
  {ok, Bin} = rocksdb:get(Db, <<"encbin">>, []),
  <<"test">> = binary_to_term(Bin),

  ok = rocksdb:merge(Db, <<"encbin">>, term_to_binary({binary_append, <<"abc">>}), []),
  {ok, Bin1} = rocksdb:get(Db, <<"encbin">>, []),
  <<"testabc">> = binary_to_term(Bin1),

  ok = rocksdb:merge(Db, <<"encbin">>, term_to_binary({binary_append, <<"de">>}), []),
  {ok, Bin2} = rocksdb:get(Db, <<"encbin">>, []),
  <<"testabcde">> = binary_to_term(Bin2),


  ok = rocksdb:close(Db),
  ok = rocksdb:destroy("/tmp/rocksdb_merge_db.test", []).
