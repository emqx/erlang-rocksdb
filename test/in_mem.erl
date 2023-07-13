%% Copyright (c) 2016-2020 Benoît Chesneau.
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
-module(in_mem).
-author("benoitc").

-compile([export_all/1]).
-include_lib("eunit/include/eunit.hrl").


basic_test() ->
  {ok, Db} = rocksdb:open("/testmem", [{env, memenv}]),
  ok = rocksdb:put(Db, <<"a">>, <<"1">>, []),
  ?assertEqual({ok, <<"1">>}, rocksdb:get(Db, <<"a">>, [])),
  {ok, Db1} = rocksdb:open("/testmem1", [{env, memenv}]),
  ok = rocksdb:put(Db1, <<"a">>, <<"2">>, []),
  ?assertEqual({ok, <<"1">>}, rocksdb:get(Db, <<"a">>, [])),
  ?assertEqual({ok, <<"2">>}, rocksdb:get(Db1, <<"a">>, [])),
  ok = rocksdb:close(Db),
  ok = rocksdb:close(Db1),
  ok.

prev_test() ->
  os:cmd("rm -rf ltest"),  % NOTE
  {ok, Ref} = rocksdb:open("/ltest", [{env, memenv}]),
  try
    rocksdb:put(Ref, <<"a">>, <<"x">>, []),
    rocksdb:put(Ref, <<"b">>, <<"y">>, []),
    {ok, I} = rocksdb:iterator(Ref, []),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I, <<>>)),
    ?assertEqual({ok, <<"b">>, <<"y">>},rocksdb:iterator_move(I, next)),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I, prev)),
    ?assertEqual(ok, rocksdb:iterator_close(I))
  after
    rocksdb:close(Ref)
  end.


env_resource_test() ->
  {ok, Env} = rocksdb:new_env(memenv),
  Options = [{env, Env}, {create_if_missing, true}],
  {ok, Db} = rocksdb:open("/testmem", Options),
  ok = rocksdb:put(Db, <<"a">>, <<"1">>, []),
  ?assertEqual({ok, <<"1">>}, rocksdb:get(Db, <<"a">>, [])),
  {ok, Db1} = rocksdb:open("/testmem1", Options),
  ok = rocksdb:put(Db1, <<"a">>, <<"2">>, []),
  ?assertEqual({ok, <<"1">>}, rocksdb:get(Db, <<"a">>, [])),
  ?assertEqual({ok, <<"2">>}, rocksdb:get(Db1, <<"a">>, [])),
  ok = rocksdb:close(Db),
  ok = rocksdb:close(Db1),
  ok.
