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
-module(snapshot).

-compile([export_all/1]).
-include_lib("eunit/include/eunit.hrl").

get_test() ->
  rocksdb_test_util:rm_rf("test.db"),
  {ok, Db} = rocksdb:open("test.db", [{create_if_missing, true}]),
  try
    rocksdb:put(Db, <<"a">>, <<"x">>, []),
    ?assertEqual({ok, <<"x">>}, rocksdb:get(Db, <<"a">>, [])),
    {ok, Snapshot} = rocksdb:snapshot(Db),
    rocksdb:put(Db, <<"a">>, <<"y">>, []),
    ?assertEqual({ok, <<"y">>}, rocksdb:get(Db, <<"a">>, [])),
    ?assertEqual({ok, <<"x">>}, rocksdb:get(Db, <<"a">>, [{snapshot, Snapshot}])),
    rocksdb:release_snapshot(Snapshot)
  after
    rocksdb:close(Db)
  end,
  destroy_and_rm("test.db").

multiple_snapshot_test() ->
  rocksdb_test_util:rm_rf("test.db"),
  {ok, Db} = rocksdb:open("test.db", [{create_if_missing, true}]),
  try
    rocksdb:put(Db, <<"a">>, <<"1">>, []),
    ?assertEqual({ok, <<"1">>}, rocksdb:get(Db, <<"a">>, [])),

    {ok, Snapshot} = rocksdb:snapshot(Db),
    rocksdb:put(Db, <<"a">>, <<"2">>, []),
    ?assertEqual({ok, <<"2">>}, rocksdb:get(Db, <<"a">>, [])),
    ?assertEqual({ok, <<"1">>}, rocksdb:get(Db, <<"a">>, [{snapshot, Snapshot}])),

    {ok, Snapshot2} = rocksdb:snapshot(Db),
    rocksdb:put(Db, <<"a">>, <<"3">>, []),
    ?assertEqual({ok, <<"3">>}, rocksdb:get(Db, <<"a">>, [])),
    ?assertEqual({ok, <<"1">>}, rocksdb:get(Db, <<"a">>, [{snapshot, Snapshot}])),
    ?assertEqual({ok, <<"2">>}, rocksdb:get(Db, <<"a">>, [{snapshot, Snapshot2}])),

    {ok, Snapshot3} = rocksdb:snapshot(Db),
    rocksdb:put(Db, <<"a">>, <<"4">>, []),
    ?assertEqual({ok, <<"4">>}, rocksdb:get(Db, <<"a">>, [])),
    ?assertEqual({ok, <<"1">>}, rocksdb:get(Db, <<"a">>, [{snapshot, Snapshot}])),
    ?assertEqual({ok, <<"2">>}, rocksdb:get(Db, <<"a">>, [{snapshot, Snapshot2}])),
    ?assertEqual({ok, <<"3">>}, rocksdb:get(Db, <<"a">>, [{snapshot, Snapshot3}])),

    rocksdb:release_snapshot(Snapshot),
    rocksdb:release_snapshot(Snapshot2),
    rocksdb:release_snapshot(Snapshot3)
  after
    rocksdb:close(Db)
  end,
  destroy_and_rm("test.db").


iterator_test() ->
  rocksdb_test_util:rm_rf("ltest"),  % NOTE
  {ok, Ref} = rocksdb:open("ltest", [{create_if_missing, true}]),
  try
    rocksdb:put(Ref, <<"a">>, <<"x">>, []),
    rocksdb:put(Ref, <<"b">>, <<"y">>, []),
    {ok, I} = rocksdb:iterator(Ref, []),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I, <<>>)),
    ?assertEqual({ok, <<"b">>, <<"y">>},rocksdb:iterator_move(I, next)),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I, prev)),

    {ok, Snapshot} = rocksdb:snapshot(Ref),

    rocksdb:put(Ref, <<"b">>, <<"z">>, []),

    {ok, I2} = rocksdb:iterator(Ref, []),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I2, <<>>)),
    ?assertEqual({ok, <<"b">>, <<"z">>},rocksdb:iterator_move(I2, next)),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I2, prev)),

    {ok, I3} = rocksdb:iterator(Ref, [{snapshot, Snapshot}]),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I3, <<>>)),
    ?assertEqual({ok, <<"b">>, <<"y">>},rocksdb:iterator_move(I3, next)),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I3, prev)),
    rocksdb:release_snapshot(Snapshot)
  after
    rocksdb:close(Ref)
  end,
  destroy_and_rm("ltest").


release_snapshot_test() ->
  rocksdb_test_util:rm_rf("ltest"),  % NOTE
  {ok, Ref} = rocksdb:open("ltest", [{create_if_missing, true}]),

  try
    rocksdb:put(Ref, <<"a">>, <<"x">>, []),
    rocksdb:put(Ref, <<"b">>, <<"y">>, []),

    {ok, Snapshot} = rocksdb:snapshot(Ref),

    rocksdb:put(Ref, <<"b">>, <<"z">>, []),

    {ok, I} = rocksdb:iterator(Ref, [{snapshot, Snapshot}]),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I, <<>>)),
    ?assertEqual({ok, <<"b">>, <<"y">>},rocksdb:iterator_move(I, next)),
    ok = rocksdb:release_snapshot(Snapshot),
    ?assertEqual({ok, <<"a">>, <<"x">>}, rocksdb:iterator_move(I, prev)),

    %% snapshot has been released, it can't be reused
    ?assertError(badarg, rocksdb:iterator(Ref, [{snapshot, Snapshot}]))

  after
    rocksdb:close(Ref)
  end,
  destroy_and_rm("ltest").


close_iterator_test() ->
  rocksdb_test_util:rm_rf("ltest"),  % NOTE
  {ok, Ref} = rocksdb:open("ltest", [{create_if_missing, true}]),

  try
    rocksdb:put(Ref, <<"a">>, <<"x">>, []),
    rocksdb:put(Ref, <<"b">>, <<"y">>, []),

    {ok, Snapshot} = rocksdb:snapshot(Ref),

    rocksdb:put(Ref, <<"b">>, <<"z">>, []),

    {ok, I} = rocksdb:iterator(Ref, [{snapshot, Snapshot}]),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I, <<>>)),
    ?assertEqual({ok, <<"b">>, <<"y">>},rocksdb:iterator_move(I, next)),

    rocksdb:iterator_close(I),
    rocksdb:release_snapshot(Snapshot)
  after
    rocksdb:close(Ref)
  end,
  destroy_and_rm("ltest").

db_close_test() ->
  rocksdb_test_util:rm_rf("ltest"),  % NOTE
  {ok, Ref} = rocksdb:open("ltest", [{create_if_missing, true}]),

  rocksdb:put(Ref, <<"a">>, <<"x">>, []),
  rocksdb:put(Ref, <<"b">>, <<"y">>, []),

  {ok, Snapshot} = rocksdb:snapshot(Ref),

  rocksdb:put(Ref, <<"b">>, <<"z">>, []),


  {ok, I} = rocksdb:iterator(Ref, [{snapshot, Snapshot}]),
  ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I, <<>>)),
  ?assertEqual({ok, <<"b">>, <<"y">>},rocksdb:iterator_move(I, next)),
  ok = rocksdb:release_snapshot(Snapshot),
  ?assertEqual({ok, <<"a">>, <<"x">>}, rocksdb:iterator_move(I, prev)),

  rocksdb:close(Ref),

  %% snapshot has been released when the db was closed, it can't be reused
  ?assertError(badarg, rocksdb:iterator(Ref, [{snapshot, Snapshot}])),


  rocksdb_test_util:rm_rf("ltest"),
  {ok, Db} = rocksdb:open("ltest", [{create_if_missing, true}]),
  rocksdb:put(Db, <<"a">>, <<"x">>, []),
  ?assertEqual({ok, <<"x">>}, rocksdb:get(Db, <<"a">>, [])),
  {ok, Snapshot2} = rocksdb:snapshot(Db),
  rocksdb:put(Db, <<"a">>, <<"y">>, []),
  ?assertEqual({ok, <<"x">>}, rocksdb:get(Db, <<"a">>, [{snapshot, Snapshot2}])),
  rocksdb:close(Db),

  %% snapshot has been released when the db was closed, it can't be reused
  ?assertError(badarg, rocksdb:get(Db, <<"a">>, [{snapshot, Snapshot2}])),
  destroy_and_rm("ltest").


cleanup_test() ->
  rocksdb_test_util:rm_rf("ltest"),  % NOTE
  {ok, Ref} = rocksdb:open("ltest", [{create_if_missing, true}]),
  rocksdb:put(Ref, <<"a">>, <<"x">>, []),
  rocksdb:put(Ref, <<"b">>, <<"y">>, []),
  Server = self(),
  Pid = spawn_link(
    fun() ->
      receive
        {db, Db} ->
          {ok, Snapshot} = rocksdb:snapshot(Db),
          ok = rocksdb:put(Db, <<"b">>, <<"z">>, []),
          Res = rocksdb:get(Db, <<"b">>, [{snapshot, Snapshot}]),
          Server ! Res
      end
    end
  ),
  Pid ! {db, Ref},
  receive
    {ok, <<"y">>} -> ok
  end,
  rocksdb:close(Ref),
  destroy_and_rm("ltest").

destroy_and_rm(Dir) ->
  rocksdb:destroy(Dir, []),
  rocksdb_test_util:rm_rf(Dir).
