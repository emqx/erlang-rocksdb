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


checkpoint_test() ->
    os:cmd("rm -rf test.db"),
    os:cmd("rm -rf test_backup.db"),
    {ok, Ref} = rocksdb:open("test.db", [{create_if_missing, true}]),
    try
        rocksdb:put(Ref, <<"a">>, <<"x">>, []),
        ?assertEqual({ok, <<"x">>}, rocksdb:get(Ref, <<"a">>, [])),
        ok = rocksdb:checkpoint(Ref, "test_backup.db"),
        ?assert(filelib:is_dir("test_backup.db")),
        rocksdb:put(Ref, <<"a">>, <<"y">>, []),
        ?assertEqual({ok, <<"y">>}, rocksdb:get(Ref, <<"a">>, []))
    after
        rocksdb:close(Ref)
    end,
    {ok, Ref2} = rocksdb:open("test_backup.db", []),
    try
        ?assertEqual({ok, <<"x">>}, rocksdb:get(Ref2, <<"a">>, []))
    after
        rocksdb:close(Ref2)
    end.

iterator_test() ->
    os:cmd("rm -rf test.db"),
    os:cmd("rm -rf test_backup.db"),
    {ok, Ref} = rocksdb:open("test.db", [{create_if_missing, true}]),
    try
        rocksdb:put(Ref, <<"a">>, <<"x">>, []),
        rocksdb:put(Ref, <<"b">>, <<"y">>, []),
        {ok, I} = rocksdb:iterator(Ref, []),
        ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I, <<>>)),
        ?assertEqual({ok, <<"b">>, <<"y">>},rocksdb:iterator_move(I, next)),
        ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I, prev)),
        ok = rocksdb:checkpoint(Ref, "test_backup.db"),
        rocksdb:put(Ref, <<"b">>, <<"z">>, []),

        {ok, I2} = rocksdb:iterator(Ref, []),
        ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I2, <<>>)),
        ?assertEqual({ok, <<"b">>, <<"z">>},rocksdb:iterator_move(I2, next)),
        ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I2, prev))
    after
        rocksdb:close(Ref)
    end,

    {ok, Ref2} = rocksdb:open("test_backup.db", []),
    try
        {ok, I3} = rocksdb:iterator(Ref2, []),
        ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I3, <<>>)),
        ?assertEqual({ok, <<"b">>, <<"y">>},rocksdb:iterator_move(I3, next)),
        ?assertEqual({ok, <<"a">>, <<"x">>},rocksdb:iterator_move(I3, prev))
    after
      rocksdb:close(Ref2)
    end.
