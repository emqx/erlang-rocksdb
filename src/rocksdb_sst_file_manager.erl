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

set_max_allowed_usage(SstFileManager, MaxAllowedSpace) when is_integer(MaxAllowedSpace) ->
    rocksdb:sst_file_manager_set(SstFileManager, "MaxAllowedSpaceUsage", MaxAllowedSpace).

set_compaction_buffer_size(SstFileManager, CompactionBufferSize) when is_integer(CompactionBufferSize) > 0 ->
    rocksdb:sst_file_manager_set(SstFileManager, "CompactionBufferSize", CompactionBufferSize).

is_max_allowed_space_reached(SstFileManager) ->
    rocksdb:sst_file_manager_is(SstFileManager, "MaxAllowedSpaceReached").

is_max_allowed_space_reached_including_compactions(SstFileManager) ->
    rocksdb:sst_file_manager_is(SstFileManager, "MaxAllowedSpaceReachedIncludingCompactions").

get_total_size(SstFileManager) ->
    rocksdb:sst_file_manager_get(SstFileManager, "TotalSize").

get_delete_rate_bytes_per_second(SstFileManager) ->
    rocksdb:sst_file_manager_get(SstFileManager, "DeleteRateBytesPerSecond").

set_delete_rate_bytes_per_second(SstFileManager, DeleteRate) ->
    rocksdb:sst_file_manager_set(SstFileManager, "DeleteRateBytesPerSecond", DeleteRate).

get_max_trash_db_ratio(SstFileManager) ->
    rocksdb:sst_file_manager_get(SstFileManager, "MaxTrashDBRatio").

set_max_trash_db_ratio(SstFileManager, Ratio) ->
    rocksdb:sst_file_manager_set(SstFileManager, "MaxTrashDBRatio", Ratio).

get_total_trash_size(SstFileManager) ->
    rocksdb:sst_file_manager_get(SstFileManager, "TotalTrashSize").
