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

-module(ttl).


-compile([export_all/1]).
-include_lib("eunit/include/eunit.hrl").

basic_test() ->
  rocksdb_test_util:rm_rf("test.db"),
  {ok, Db} = rocksdb:open_with_ttl("test.db", [{create_if_missing, true}], 1, false),
  ?assertEqual({ok, ["default"]}, rocksdb:list_column_families("test.db", [])),
  ok = rocksdb:put(Db, <<"a">>, <<"a1">>, []),
  {ok,  <<"a1">>} = rocksdb:get(Db, <<"a">>, []),
  timer:sleep(4000),
  rocksdb:compact_range(Db, <<"0">>, <<"b">>, []),
  not_found = rocksdb:get(Db, <<"a">>, []),
  rocksdb:close(Db),
  rocksdb:destroy("test.db", []),
  rocksdb_test_util:rm_rf("test.db"),
  ok.

