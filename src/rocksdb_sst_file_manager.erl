%%% -*- erlang -*-
%%
%% Copyright (c) 2018 Benoit Chesneau
%%
%% Licensed under the Apache License, Version 2.0 (the "License");
%% you may not use this file except in compliance with the License.
%% You may obtain a copy of the License at
%%
%% http://www.apache.org/licenses/LICENSE-2.0
%%
%% Unless required by applicable law or agreed to in writing, software
%% distributed under the License is distributed on an "AS IS" BASIS,
%% WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
%% See the License for the specific language governing permissions and
%% limitations under the License.


-module(rocksdb_sst_file_manager).

-export([
    set_max_allowed_usage/2,
    set_compaction_buffer_size/2,
    is_max_allowed_space_reached/1,
    is_max_allowed_space_reached_including_compactions/1,
    get_total_size/1,
    get_delete_rate_bytes_per_second/1,
    set_delete_rate_bytes_per_second/2,
    get_max_trash_db_ratio/1,
    set_max_trash_db_ratio/2,
    get_total_trash_size/1
]).


%% @doc Update the maximum allowed space that should be used by RocksDB, if
%% the total size of the SST files exceeds MaxAllowedSpace, writes to
%% RocksDB will fail.
%%
%% setting MaxAllowedSpace to 0 will disable this feature; maximum allowed
%% pace will be infinite (Default value).
-spec set_max_allowed_usage(rocksdb:sst_file_manager(), non_neg_integer()) -> ok.
set_max_allowed_usage(SstFileManager, MaxAllowedSpace) when is_integer(MaxAllowedSpace) ->
    rocksdb:sst_file_manager_set(SstFileManager, "MaxAllowedSpaceUsage", MaxAllowedSpace).

%% @doc Set the amount of buffer room each compaction should be able to leave.
%%  In other words, at its maximum disk space consumption, the compaction
%% should still leave compaction_buffer_size available on the disk so that
%% other background functions may continue, such as logging and flushing.
-spec set_compaction_buffer_size(rocksdb:sst_file_manager(), non_neg_integer()) -> ok.
set_compaction_buffer_size(SstFileManager, CompactionBufferSize) when is_integer(CompactionBufferSize) > 0 ->
    rocksdb:sst_file_manager_set(SstFileManager, "CompactionBufferSize", CompactionBufferSize).

%% @doc Return true if the total size of SST files exceeded the maximum allowed
%% space usage.
-spec is_max_allowed_space_reached(rocksdb:sst_file_manager()) -> boolean().
is_max_allowed_space_reached(SstFileManager) ->
    rocksdb:sst_file_manager_is(SstFileManager, "MaxAllowedSpaceReached").

%% @doc Returns true if the total size of SST files as well as estimated size
%% of ongoing compactions exceeds the maximums allowed space usage.
-spec is_max_allowed_space_reached_including_compactions(rocksdb:sst_file_manager()) -> boolean().
is_max_allowed_space_reached_including_compactions(SstFileManager) ->
    rocksdb:sst_file_manager_is(SstFileManager, "MaxAllowedSpaceReachedIncludingCompactions").

%% @doc Return the total size of all tracked files.
-spec get_total_size(rocksdb:sst_file_manager()) -> non_neg_integer().
get_total_size(SstFileManager) ->
    rocksdb:sst_file_manager_get(SstFileManager, "TotalSize").

%% @doc Return delete rate limit in bytes per second.
-spec get_delete_rate_bytes_per_second(rocksdb:sst_file_manager()) -> non_neg_integer().
get_delete_rate_bytes_per_second(SstFileManager) ->
    rocksdb:sst_file_manager_get(SstFileManager, "DeleteRateBytesPerSecond").

%% @doc Update the delete rate limit in bytes per second.
%% zero means disable delete rate limiting and delete files immediately
-spec set_delete_rate_bytes_per_second(rocksdb:sst_file_manager(), non_neg_integer()) -> ok.
set_delete_rate_bytes_per_second(SstFileManager, DeleteRate) ->
    rocksdb:sst_file_manager_set(SstFileManager, "DeleteRateBytesPerSecond", DeleteRate).

%% @doc Return trash/DB size ratio where new files will be deleted immediately
-spec get_max_trash_db_ratio(rocksdb:sst_file_manager()) -> float().
get_max_trash_db_ratio(SstFileManager) ->
    rocksdb:sst_file_manager_get(SstFileManager, "MaxTrashDBRatio").

%% @doc Update trash/DB size ratio where new files will be deleted immediately
-spec set_max_trash_db_ratio(rocksdb:sst_file_manager(), float()) -> ok.
set_max_trash_db_ratio(SstFileManager, Ratio) ->
    rocksdb:sst_file_manager_set(SstFileManager, "MaxTrashDBRatio", Ratio).

%% @doc Return the total size of trash files
-spec get_total_trash_size(rocksdb:sst_file_manager()) -> non_neg_integer().
get_total_trash_size(SstFileManager) ->
    rocksdb:sst_file_manager_get(SstFileManager, "TotalTrashSize").
