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

-module(rocksdb_write_buffer_manager).

-export([
    enabled/1,
    memory_usage/1,
    mutable_memtable_memory_usage/1,
    buffer_size/1
]).

enabled(WriteBufferManager) ->
    rocksdb:write_buffer_manager_is_enabled(WriteBufferManager).

memory_usage(WriteBufferManager) ->
    rocksdb:write_buffer_manager_get(WriteBufferManager, "memory_usage").

mutable_memtable_memory_usage(WriteBufferManager) ->
    rocksdb:write_buffer_manager_get(WriteBufferManager, "mutable_memtable_memory_usage").

buffer_size(WriteBufferManager) ->
    rocksdb:write_buffer_manager_get(WriteBufferManager, "buffer_size").

