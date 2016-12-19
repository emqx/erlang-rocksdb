%%%-------------------------------------------------------------------
%%% @author benoitc
%%% @copyright (C) 2016, Benoit Chesneau
%%% @doc
%%%
%%% @end
%%% Created : 21. Apr 2016 21:49
%%%-------------------------------------------------------------------
-module(in_mem).
-author("benoitc").

-compile([export_all/1]).
-include_lib("eunit/include/eunit.hrl").


basic_test() ->
  {ok, Db} = rocksdb:open("test", [{in_memory, true}]),
  ok = rocksdb:put(Db, <<"a">>, <<"1">>, []),
  ?assertEqual({ok, <<"1">>}, rocksdb:get(Db, <<"a">>, [])),
  {ok, Db1} = rocksdb:open("test1", [{in_memory, true}]),
  ok = rocksdb:put(Db1, <<"a">>, <<"2">>, []),
  ?assertEqual({ok, <<"1">>}, rocksdb:get(Db, <<"a">>, [])),
  ?assertEqual({ok, <<"2">>}, rocksdb:get(Db1, <<"a">>, [])),
  ok = rocksdb:close(Db),
  ok = rocksdb:close(Db1),
  ok.
