%% Copyright (c) 2026 EMQX.
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
-module(db_options).

-include_lib("eunit/include/eunit.hrl").

%% Verify that various DB options supported by `rocksdb:open/2` take effect
%% on the RocksDB layer.
open_options_test_() ->
  DBName = "erocksdb.open_options_test.test",
  {setup, fun() -> DBName end, fun rocksdb_test_util:rm_rf/1, fun() ->
    {ok, Ref} = rocksdb:open(DBName,
                            [ {create_if_missing, true}
                            , {paranoid_checks, false}                    % boolean()
                            , {max_open_files, 1337}                      % integer()
                            , {max_total_wal_size, 16 bsl 20}             % non_neg_integer()
                            , {use_fsync, false}                          % boolean()
                            , {db_log_dir, filename:join(DBName, "log")}  % file:filename_all()
                            , {wal_dir, filename:join(DBName, "wal")}     % file:filename_all()
                            , {delete_obsolete_files_period_micros, 60_000_000} % pos_integer()
                            , {max_background_jobs, 3}                    % pos_integer()
                            , {max_background_compactions, 4}             % pos_integer()
                            , {max_background_flushes, 5}                 % pos_integer()
                            , {max_log_file_size, 1 bsl 20}               % non_neg_integer()
                            , {log_file_time_to_roll, 60}                 % non_neg_integer()
                            , {keep_log_file_num, 6}                      % pos_integer()
                            , {max_manifest_file_size, 2 bsl 20}          % pos_integer()
                            , {table_cache_numshardbits, 8}               % pos_integer()
                            , {wal_ttl_seconds, 600}                      % non_neg_integer()
                            , {manual_wal_flush, true}                    % boolean()
                            , {wal_size_limit_mb, 32}                     % non_neg_integer()
                            , {manifest_preallocation_size, 1 bsl 20}     % pos_integer()
                            , {allow_mmap_reads, true}                    % boolean()
                            , {allow_mmap_writes, true}                   % boolean()
                            , {is_fd_close_on_exec, true}                 % boolean()
                            , {advise_random_on_open, false}              % boolean()
                            , {compaction_readahead_size, 4 bsl 20}       % non_neg_integer()
                            , {use_adaptive_mutex, true}                  % boolean()
                            , {bytes_per_sync, 100 bsl 10}                % non_neg_integer()
                            , {skip_stats_update_on_db_open, true}        % boolean()
                            , {wal_recovery_mode, point_in_time_recovery} % wal_recovery_mode()
                            , {allow_concurrent_memtable_write, false}    % boolean()
                            , {enable_write_thread_adaptive_yield, false} % boolean()
                            , {db_write_buffer_size, 5 bsl 20}            % non_neg_integer()
                            , {max_subcompactions, 3}                     % non_neg_integer()
                            , {atomic_flush, true}                        % boolean()
                            , {two_write_queues, true}                    % boolean()
                            , {allow_fallocate, false}                    % boolean()
                          % Mutually exclusive with above ones:
                          % , {use_direct_reads, true}
                          % , {use_direct_io_for_flush_and_compaction, true}
                          % , {enable_pipelined_write, true}
                          % , {unordered_write, true}
                            ]),
    % Perform smoke read-write cycle:
    true = rocksdb:is_empty(Ref),
    ok = rocksdb:put(Ref, <<"abc">>, <<"123">>, []),
    false = rocksdb:is_empty(Ref),
    {ok, <<"123">>} = rocksdb:get(Ref, <<"abc">>, []),
    ok = rocksdb:close(Ref),
    % Verify custom directories took effect:
    ?assertMatch({ok, [_ | _]}, file:list_dir(filename:join(DBName, "wal"))),
    ?assertMatch({ok, [_ | _]}, file:list_dir(filename:join(DBName, "log"))),
    % Verify that RocksDB runs with the specified options:
    OptionsParsed = consult_db_options(DBName),
    DBOptions = maps:get(db_options, OptionsParsed),
    DBOptionDiff = [{Opt, V, '=/=', VE} ||
                    {Opt, VE} <- [ {paranoid_checks, false}
                                  , {keep_log_file_num, 6}
                                  , {max_open_files, 1337}
                                  , {max_total_wal_size, 16 bsl 20}
                                  , {use_fsync, false}
                                  , {delete_obsolete_files_period_micros, 60 * 1000 * 1000}
                                  , {max_background_jobs, 3}
                                  , {max_background_compactions, 4}
                                  , {max_background_flushes, 5}
                                  , {max_log_file_size, 1 bsl 20}
                                  , {log_file_time_to_roll, 60}
                                  , {max_manifest_file_size, 2 bsl 20}
                                  , {table_cache_numshardbits, 8}
                                  , {'WAL_ttl_seconds', 600}
                                  , {'WAL_size_limit_MB', 32}
                                  , {manual_wal_flush, true}
                                  , {manifest_preallocation_size, 1 bsl 20}
                                  , {allow_mmap_reads, true}
                                  , {allow_mmap_writes, true}
                                  , {is_fd_close_on_exec, true}
                                  , {advise_random_on_open, false}
                                  , {compaction_readahead_size, 4 bsl 20}
                                  , {use_adaptive_mutex, true}
                                  , {bytes_per_sync, 100 bsl 10}
                                  , {skip_stats_update_on_db_open, true}
                                  , {wal_recovery_mode, <<"kPointInTimeRecovery">>}
                                  , {allow_concurrent_memtable_write, false}
                                  , {enable_write_thread_adaptive_yield, false}
                                  , {db_write_buffer_size, 5 bsl 20}
                                  , {max_subcompactions, 3}
                                  , {atomic_flush, true}
                                  , {two_write_queues, true}
                                  , {allow_fallocate, false}
                                  ],
                    V <- [maps:get(Opt, DBOptions, undefined)],
                    V =/= VE],
    ?assertEqual([], DBOptionDiff, OptionsParsed)
  end}.

consult_db_options(DBName) ->
  OptionsFilePat = filename:join(DBName, "OPTIONS-*"),
  OptionsFilename = lists:last(lists:sort(filelib:wildcard(OptionsFilePat))),
  {ok, Contents} = file:read_file(OptionsFilename),
  parse_db_options(Contents).

%% Ad-hoc INI file parser.
%% Suitable for rudimentary parsing of RocksDB's OPTIONS files.

parse_db_options(Bytes) ->
  OptionsLines = [string:trim(L) || L <- string:split(Bytes, "\n", all)],
  Parsed = lists:foldl(fun parse_db_options/2, #{}, OptionsLines),
  maps:remove(section, Parsed).

parse_db_options(<<>>, Acc) ->
  Acc;
parse_db_options(<<"#", _/bytes>>, Acc) ->
  Acc;
parse_db_options(<<"[", _/bytes>> = L, Acc) ->
  <<"[", Section:(byte_size(L) - 2)/bytes, "]">> = L,
  case Section of
    <<"Version">> ->
      Acc#{section => version};
    <<"DBOptions">> ->
      Acc#{section => db_options};
    <<"CFOptions ", CFName/bytes>> ->
      Acc#{section => {cf_options, parse_unquote(CFName)}};
    _ ->
      Acc#{section => {raw, Section}}
  end;
parse_db_options(L, Acc) ->
  SectionName = maps:get(section, Acc),
  Section = maps:get(SectionName, Acc, #{}),
  [NameS, ValueS] = string:split(L, "="),
  Name = binary_to_atom(NameS),
  Value = parse_option_value(ValueS),
  Acc#{SectionName => maps:put(Name, Value, Section)}.

parse_unquote(Quoted) ->
  {ok, [{string, _, String}], _} = erl_scan:string(unicode:characters_to_list(Quoted)),
  String.

parse_option_value(<<"false">>) -> false;
parse_option_value(<<"true">>) -> true;
parse_option_value(<<"nullptr">>) -> undefined;
%% Parse as integer if looks like a number, otherwise return as is.
parse_option_value(String) ->
  try binary_to_integer(String)
  catch error:_ -> String end.
