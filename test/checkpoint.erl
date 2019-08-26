%% Copyright (c) 2016 Benoît Chesneau.
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

-module(checkpoint).
-compile([export_all/1]).
-include_lib("eunit/include/eunit.hrl").


-define(DB, "test.db").
-define(BACKUP_DB, "test_backup.db").


checkpoint_test() ->
  clean_dbs(),
  {ok, Ref} = rocksdb:open(?DB, [{create_if_missing, true}]),
  try
    rocksdb:put(Ref, <<"a">>, <<"x">>, []),
    ?assertEqual({ok, <<"x">>}, rocksdb:get(Ref, <<"a">>, [])),
    ok = rocksdb:checkpoint(Ref, ?BACKUP_DB),
    ?assert(filelib:is_dir(?BACKUP_DB)),
    rocksdb:put(Ref, <<"a">>, <<"y">>, []),
    ?assertEqual({ok, <<"y">>}, rocksdb:get(Ref, <<"a">>, []))
  after
    rocksdb:close(Ref)
  end,
  {ok, Ref2} = rocksdb:open(?BACKUP_DB, []),
  try
    ?assertEqual({ok, <<"x">>}, rocksdb:get(Ref2, <<"a">>, []))
  after
    rocksdb:close(Ref2),
    clean_dbs()
  end.

iterator_test() ->
  clean_dbs(),
  {ok, Ref} = rocksdb:open(?DB, [{create_if_missing, true}]),
  try
    rocksdb:put(Ref, <<"a">>, <<"x">>, []),
    rocksdb:put(Ref, <<"b">>, <<"y">>, []),
    {ok, I} = rocksdb:iterator(Ref, []),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I, <<>>)),
    ?assertEqual({ok, <<"b">>, <<"y">>},rocksdb:iterator_move(I, next)),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I, prev)),
    ok = rocksdb:checkpoint(Ref, ?BACKUP_DB),
    rocksdb:put(Ref, <<"b">>, <<"z">>, []),

    {ok, I2} = rocksdb:iterator(Ref, []),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I2, <<>>)),
    ?assertEqual({ok, <<"b">>, <<"z">>},rocksdb:iterator_move(I2, next)),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I2, prev))
  after
    rocksdb:close(Ref)
  end,
  {ok, Ref2} = rocksdb:open(?BACKUP_DB, []),
  try
    {ok, I3} = rocksdb:iterator(Ref2, []),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I3, <<>>)),
    ?assertEqual({ok, <<"b">>, <<"y">>},rocksdb:iterator_move(I3, next)),
    ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I3, prev))
  after
    rocksdb:close(Ref2),
    clean_dbs()
  end.


clean_dbs() ->
  rocksdb_test_util:rm_rf(?DB),
  rocksdb_test_util:rm_rf(?BACKUP_DB).

