

# Module rocksdb #
* [Description](#description)
* [Data Types](#types)
* [Function Index](#index)
* [Function Details](#functions)

Erlang Wrapper for RocksDB.

<a name="types"></a>

## Data Types ##




### <a name="type-access_hint">access_hint()</a> ###


<pre><code>
access_hint() = normal | sequential | willneed | none
</code></pre>




### <a name="type-block_based_table_options">block_based_table_options()</a> ###


<pre><code>
block_based_table_options() = [{no_block_cache, boolean()} | {block_size, pos_integer()} | {block_cache_size, pos_integer()} | {bloom_filter_policy, BitsPerKey::pos_integer()} | {format_version, 0 | 1 | 2} | {skip_table_builder_flush, boolean()} | {cache_index_and_filter_blocks, boolean()}]
</code></pre>




### <a name="type-cf_handle">cf_handle()</a> ###


__abstract datatype__: `cf_handle()`




### <a name="type-cf_options">cf_options()</a> ###


<pre><code>
cf_options() = [{block_cache_size_mb_for_point_lookup, non_neg_integer()} | {memtable_memory_budget, pos_integer()} | {write_buffer_size, pos_integer()} | {max_write_buffer_number, pos_integer()} | {min_write_buffer_number_to_merge, pos_integer()} | {compression, <a href="#type-compression_type">compression_type()</a>} | {num_levels, pos_integer()} | {level0_file_num_compaction_trigger, integer()} | {level0_slowdown_writes_trigger, integer()} | {level0_stop_writes_trigger, integer()} | {max_mem_compaction_level, pos_integer()} | {target_file_size_base, pos_integer()} | {target_file_size_multiplier, pos_integer()} | {max_bytes_for_level_base, pos_integer()} | {max_bytes_for_level_multiplier, pos_integer()} | {max_compaction_bytes, pos_integer()} | {soft_rate_limit, float()} | {hard_rate_limit, float()} | {arena_block_size, integer()} | {disable_auto_compactions, boolean()} | {purge_redundant_kvs_while_flush, boolean()} | {compaction_style, <a href="#type-compaction_style">compaction_style()</a>} | {verify_checksums_in_compaction, boolean()} | {filter_deletes, boolean()} | {max_sequential_skip_in_iterations, pos_integer()} | {inplace_update_support, boolean()} | {inplace_update_num_locks, pos_integer()} | {table_factory_block_cache_size, pos_integer()} | {in_memory_mode, boolean()} | {block_based_table_options, <a href="#type-block_based_table_options">block_based_table_options()</a>}]
</code></pre>




### <a name="type-compaction_style">compaction_style()</a> ###


<pre><code>
compaction_style() = level | universal | fifo | none
</code></pre>




### <a name="type-compression_type">compression_type()</a> ###


<pre><code>
compression_type() = snappy | zlib | bzip2 | lz4 | lz4h | none
</code></pre>




### <a name="type-db_handle">db_handle()</a> ###


__abstract datatype__: `db_handle()`




### <a name="type-db_options">db_options()</a> ###


<pre><code>
db_options() = [{total_threads, pos_integer()} | {create_if_missing, boolean()} | {create_missing_column_families, boolean()} | {error_if_exists, boolean()} | {paranoid_checks, boolean()} | {max_open_files, integer()} | {max_total_wal_size, non_neg_integer()} | {disable_data_sync, boolean()} | {use_fsync, boolean()} | {db_paths, [#db_path{path = <a href="file.md#type-filename_all">file:filename_all()</a>, target_size = non_neg_integer()}]} | {db_log_dir, <a href="file.md#type-filename_all">file:filename_all()</a>} | {wal_dir, <a href="file.md#type-filename_all">file:filename_all()</a>} | {delete_obsolete_files_period_micros, pos_integer()} | {max_background_compactions, pos_integer()} | {max_background_flushes, pos_integer()} | {max_log_file_size, non_neg_integer()} | {log_file_time_to_roll, non_neg_integer()} | {keep_log_file_num, pos_integer()} | {max_manifest_file_size, pos_integer()} | {table_cache_numshardbits, pos_integer()} | {wal_ttl_seconds, non_neg_integer()} | {wal_size_limit_mb, non_neg_integer()} | {manifest_preallocation_size, pos_integer()} | {allow_os_buffer, boolean()} | {allow_mmap_reads, boolean()} | {allow_mmap_writes, boolean()} | {is_fd_close_on_exec, boolean()} | {skip_log_error_on_recovery, boolean()} | {stats_dump_period_sec, non_neg_integer()} | {advise_random_on_open, boolean()} | {access_hint, <a href="#type-access_hint">access_hint()</a>} | {compaction_readahead_size, non_neg_integer()} | {use_adaptive_mutex, boolean()} | {bytes_per_sync, non_neg_integer()} | {skip_stats_update_on_db_open, boolean()} | {wal_recovery_mode, <a href="#type-wal_recovery_mode">wal_recovery_mode()</a>} | {allow_concurrent_memtable_write, boolean()} | {enable_write_thread_adaptive_yield, boolean()} | {in_memory, boolean()}]
</code></pre>




### <a name="type-fold_fun">fold_fun()</a> ###


<pre><code>
fold_fun() = fun(({Key::binary(), Value::binary()}, any()) -&gt; any())
</code></pre>




### <a name="type-fold_keys_fun">fold_keys_fun()</a> ###


<pre><code>
fold_keys_fun() = fun((Key::binary(), any()) -&gt; any())
</code></pre>




### <a name="type-iterator_action">iterator_action()</a> ###


<pre><code>
iterator_action() = first | last | next | prev | binary()
</code></pre>




### <a name="type-itr_handle">itr_handle()</a> ###


__abstract datatype__: `itr_handle()`




### <a name="type-read_options">read_options()</a> ###


<pre><code>
read_options() = [{verify_checksums, boolean()} | {fill_cache, boolean()} | {iterate_upper_bound, binary()} | {tailing, boolean()} | {total_order_seek, boolean()} | {snapshot, <a href="#type-snapshot_handle">snapshot_handle()</a>}]
</code></pre>




### <a name="type-snapshot_handle">snapshot_handle()</a> ###


__abstract datatype__: `snapshot_handle()`




### <a name="type-wal_recovery_mode">wal_recovery_mode()</a> ###


<pre><code>
wal_recovery_mode() = tolerate_corrupted_tail_records | absolute_consistency | point_in_time_recovery | skip_any_corrupted_records
</code></pre>




### <a name="type-write_actions">write_actions()</a> ###


<pre><code>
write_actions() = [{put, Key::binary(), Value::binary()} | {put, ColumnFamilyHandle::<a href="#type-cf_handle">cf_handle()</a>, Key::binary(), Value::binary()} | {delete, Key::binary()} | {delete, ColumnFamilyHandle::<a href="#type-cf_handle">cf_handle()</a>, Key::binary()} | clear]
</code></pre>




### <a name="type-write_options">write_options()</a> ###


<pre><code>
write_options() = [{sync, boolean()} | {disable_wal, boolean()} | {timeout_hint_us, non_neg_integer()} | {ignore_missing_column_families, boolean()}]
</code></pre>

<a name="index"></a>

## Function Index ##


<table width="100%" border="1" cellspacing="0" cellpadding="2" summary="function index"><tr><td valign="top"><a href="#checkpoint-2">checkpoint/2</a></td><td>take a snapshot of a running RocksDB database in a separate directory
http://rocksdb.org/blog/2609/use-checkpoints-for-efficient-snapshots/.</td></tr><tr><td valign="top"><a href="#close-1">close/1</a></td><td>Close RocksDB.</td></tr><tr><td valign="top"><a href="#count-1">count/1</a></td><td>Return the approximate number of keys in the default column family.</td></tr><tr><td valign="top"><a href="#count-2">count/2</a></td><td>
Return the approximate number of keys in the specified column family.</td></tr><tr><td valign="top"><a href="#create_column_family-3">create_column_family/3</a></td><td>Create a new column family.</td></tr><tr><td valign="top"><a href="#delete-3">delete/3</a></td><td>Delete a key/value pair in the default column family.</td></tr><tr><td valign="top"><a href="#delete-4">delete/4</a></td><td>Delete a key/value pair in the specified column family.</td></tr><tr><td valign="top"><a href="#destroy-2">destroy/2</a></td><td>Destroy the contents of the specified database.</td></tr><tr><td valign="top"><a href="#drop_column_family-1">drop_column_family/1</a></td><td>Drop a column family.</td></tr><tr><td valign="top"><a href="#fold-4">fold/4</a></td><td>Calls Fun(Elem, AccIn) on successive elements in the default column family
starting with AccIn == Acc0.</td></tr><tr><td valign="top"><a href="#fold-5">fold/5</a></td><td>Calls Fun(Elem, AccIn) on successive elements in the specified column family
Other specs are same with fold/4.</td></tr><tr><td valign="top"><a href="#fold_keys-4">fold_keys/4</a></td><td>Calls Fun(Elem, AccIn) on successive elements in the default column family
starting with AccIn == Acc0.</td></tr><tr><td valign="top"><a href="#fold_keys-5">fold_keys/5</a></td><td>Calls Fun(Elem, AccIn) on successive elements in the specified column family
Other specs are same with fold_keys/4.</td></tr><tr><td valign="top"><a href="#get-3">get/3</a></td><td>Retrieve a key/value pair in the default column family.</td></tr><tr><td valign="top"><a href="#get-4">get/4</a></td><td>Retrieve a key/value pair in the specified column family.</td></tr><tr><td valign="top"><a href="#get_property-2">get_property/2</a></td><td>Return the RocksDB internal status of the default column family specified at Property.</td></tr><tr><td valign="top"><a href="#get_property-3">get_property/3</a></td><td>Return the RocksDB internal status of the specified column family specified at Property.</td></tr><tr><td valign="top"><a href="#is_empty-1">is_empty/1</a></td><td>is the database empty.</td></tr><tr><td valign="top"><a href="#iterator-2">iterator/2</a></td><td>Return a iterator over the contents of the database.</td></tr><tr><td valign="top"><a href="#iterator-3">iterator/3</a></td><td></td></tr><tr><td valign="top"><a href="#iterator_close-1">iterator_close/1</a></td><td>Close a iterator.</td></tr><tr><td valign="top"><a href="#iterator_move-2">iterator_move/2</a></td><td> Move to the specified place.</td></tr><tr><td valign="top"><a href="#iterators-3">iterators/3</a></td><td>Return a iterator over the contents of the specified column families.</td></tr><tr><td valign="top"><a href="#iterators-4">iterators/4</a></td><td>Return keys iterator over the contents of the specified column families.</td></tr><tr><td valign="top"><a href="#list_column_families-2">list_column_families/2</a></td><td>List column families.</td></tr><tr><td valign="top"><a href="#open-2">open/2</a></td><td>Open RocksDB with the defalut column family.</td></tr><tr><td valign="top"><a href="#open_with_cf-3">open_with_cf/3</a></td><td>Open RocksDB with the specified column families.</td></tr><tr><td valign="top"><a href="#put-4">put/4</a></td><td>Put a key/value pair into the default column family.</td></tr><tr><td valign="top"><a href="#put-5">put/5</a></td><td>Put a key/value pair into the specified column family.</td></tr><tr><td valign="top"><a href="#release_snapshot-1">release_snapshot/1</a></td><td>release a snapshot.</td></tr><tr><td valign="top"><a href="#repair-2">repair/2</a></td><td>Try to repair as much of the contents of the database as possible.</td></tr><tr><td valign="top"><a href="#snapshot-1">snapshot/1</a></td><td>return a database snapshot
Snapshots provide consistent read-only views over the entire state of the key-value store.</td></tr><tr><td valign="top"><a href="#stats-1">stats/1</a></td><td>Return the current stats of the default column family
Implemented by calling GetProperty with "rocksdb.stats".</td></tr><tr><td valign="top"><a href="#stats-2">stats/2</a></td><td>Return the current stats of the specified column family
Implemented by calling GetProperty with "rocksdb.stats".</td></tr><tr><td valign="top"><a href="#write-3">write/3</a></td><td>Apply the specified updates to the database.</td></tr></table>


<a name="functions"></a>

## Function Details ##

<a name="checkpoint-2"></a>

### checkpoint/2 ###

<pre><code>
checkpoint(DbHandle::<a href="#type-db_handle">db_handle()</a>, Path::<a href="file.md#type-filename_all">file:filename_all()</a>) -&gt; ok | {error, any()}
</code></pre>
<br />

take a snapshot of a running RocksDB database in a separate directory
http://rocksdb.org/blog/2609/use-checkpoints-for-efficient-snapshots/

<a name="close-1"></a>

### close/1 ###

<pre><code>
close(DBHandle) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

Close RocksDB

<a name="count-1"></a>

### count/1 ###

<pre><code>
count(DBHandle::<a href="#type-db_handle">db_handle()</a>) -&gt; non_neg_integer() | {error, any()}
</code></pre>
<br />

Return the approximate number of keys in the default column family.
Implemented by calling GetIntProperty with "rocksdb.estimate-num-keys"

<a name="count-2"></a>

### count/2 ###

<pre><code>
count(DBHandle::<a href="#type-db_handle">db_handle()</a>, CFHandle::<a href="#type-cf_handle">cf_handle()</a>) -&gt; non_neg_integer() | {error, any()}
</code></pre>
<br />

Return the approximate number of keys in the specified column family.

<a name="create_column_family-3"></a>

### create_column_family/3 ###

<pre><code>
create_column_family(DBHandle, Name, CFOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>Name = string()</code></li><li><code>CFOpts = <a href="#type-cf_options">cf_options()</a></code></li><li><code>Res = {ok, <a href="#type-cf_handle">cf_handle()</a>} | {error, any()}</code></li></ul>

Create a new column family

<a name="delete-3"></a>

### delete/3 ###

<pre><code>
delete(DBHandle, Key, WriteOpts) -&gt; ok | {error, any()}
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>Key = binary()</code></li><li><code>WriteOpts = <a href="#type-write_options">write_options()</a></code></li></ul>

Delete a key/value pair in the default column family

<a name="delete-4"></a>

### delete/4 ###

<pre><code>
delete(DBHandle, CFHandle, Key, WriteOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>Key = binary()</code></li><li><code>WriteOpts = <a href="#type-write_options">write_options()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

Delete a key/value pair in the specified column family

<a name="destroy-2"></a>

### destroy/2 ###

<pre><code>
destroy(Name::<a href="file.md#type-filename_all">file:filename_all()</a>, DBOpts::<a href="#type-db_options">db_options()</a>) -&gt; ok | {error, any()}
</code></pre>
<br />

Destroy the contents of the specified database.
Be very careful using this method.

<a name="drop_column_family-1"></a>

### drop_column_family/1 ###

<pre><code>
drop_column_family(CFHandle) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

Drop a column family

<a name="fold-4"></a>

### fold/4 ###

<pre><code>
fold(DBHandle, Fun, AccIn, ReadOpts) -&gt; AccOut
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>Fun = <a href="#type-fold_fun">fold_fun()</a></code></li><li><code>AccIn = any()</code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>AccOut = any()</code></li></ul>

Calls Fun(Elem, AccIn) on successive elements in the default column family
starting with AccIn == Acc0.
Fun/2 must return a new accumulator which is passed to the next call.
The function returns the final value of the accumulator.
Acc0 is returned if the default column family is empty.

<a name="fold-5"></a>

### fold/5 ###

<pre><code>
fold(DBHandle, CFHandle, Fun, AccIn, ReadOpts) -&gt; AccOut
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>Fun = <a href="#type-fold_fun">fold_fun()</a></code></li><li><code>AccIn = any()</code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>AccOut = any()</code></li></ul>

Calls Fun(Elem, AccIn) on successive elements in the specified column family
Other specs are same with fold/4

<a name="fold_keys-4"></a>

### fold_keys/4 ###

<pre><code>
fold_keys(DBHandle, Fun, AccIn, ReadOpts) -&gt; AccOut
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>Fun = <a href="#type-fold_keys_fun">fold_keys_fun()</a></code></li><li><code>AccIn = any()</code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>AccOut = any()</code></li></ul>

Calls Fun(Elem, AccIn) on successive elements in the default column family
starting with AccIn == Acc0.
Fun/2 must return a new accumulator which is passed to the next call.
The function returns the final value of the accumulator.
Acc0 is returned if the default column family is empty.

<a name="fold_keys-5"></a>

### fold_keys/5 ###

<pre><code>
fold_keys(DBHandle, CFHandle, Fun, AccIn, ReadOpts) -&gt; AccOut
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>Fun = <a href="#type-fold_keys_fun">fold_keys_fun()</a></code></li><li><code>AccIn = any()</code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>AccOut = any()</code></li></ul>

Calls Fun(Elem, AccIn) on successive elements in the specified column family
Other specs are same with fold_keys/4

<a name="get-3"></a>

### get/3 ###

<pre><code>
get(DBHandle, Key, ReadOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>Key = binary()</code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>Res = {ok, binary()} | not_found | {error, any()}</code></li></ul>

Retrieve a key/value pair in the default column family

<a name="get-4"></a>

### get/4 ###

<pre><code>
get(DBHandle, CFHandle, Key, ReadOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>Key = binary()</code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>Res = {ok, binary()} | not_found | {error, any()}</code></li></ul>

Retrieve a key/value pair in the specified column family

<a name="get_property-2"></a>

### get_property/2 ###

<pre><code>
get_property(DBHandle::<a href="#type-db_handle">db_handle()</a>, Property::binary()) -&gt; {ok, any()} | {error, any()}
</code></pre>
<br />

Return the RocksDB internal status of the default column family specified at Property

<a name="get_property-3"></a>

### get_property/3 ###

<pre><code>
get_property(DBHandle::<a href="#type-db_handle">db_handle()</a>, CFHandle::<a href="#type-cf_handle">cf_handle()</a>, Property::binary()) -&gt; string() | {error, any()}
</code></pre>
<br />

Return the RocksDB internal status of the specified column family specified at Property

<a name="is_empty-1"></a>

### is_empty/1 ###

<pre><code>
is_empty(DBHandle::<a href="#type-db_handle">db_handle()</a>) -&gt; true | false
</code></pre>
<br />

is the database empty

<a name="iterator-2"></a>

### iterator/2 ###

<pre><code>
iterator(DBHandle, ReadOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>Res = {ok, <a href="#type-itr_handle">itr_handle()</a>} | {error, any()}</code></li></ul>

Return a iterator over the contents of the database.
The result of iterator() is initially invalid (caller must
call iterator_move function on the iterator before using it).

<a name="iterator-3"></a>

### iterator/3 ###

<pre><code>
iterator(DBHandle, ReadOpts, X3::keys_only) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>Res = {ok, <a href="#type-itr_handle">itr_handle()</a>} | {error, any()}</code></li></ul>

<a name="iterator_close-1"></a>

### iterator_close/1 ###

<pre><code>
iterator_close(ITRHandle::<a href="#type-itr_handle">itr_handle()</a>) -&gt; ok
</code></pre>
<br />

Close a iterator

<a name="iterator_move-2"></a>

### iterator_move/2 ###

<pre><code>
iterator_move(ITRHandle, ITRAction) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>ITRHandle = <a href="#type-itr_handle">itr_handle()</a></code></li><li><code>ITRAction = <a href="#type-iterator_action">iterator_action()</a></code></li><li><code>Res = {ok, Key::binary(), Value::binary()} | {ok, Key::binary()} | {error, invalid_iterator} | {error, iterator_closed}</code></li></ul>

Move to the specified place

<a name="iterators-3"></a>

### iterators/3 ###

<pre><code>
iterators(DBHandle, CFHandles, ReadOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandles = [<a href="#type-cf_handle">cf_handle()</a>]</code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>Res = {ok, <a href="#type-itr_handle">itr_handle()</a>} | {error, any()}</code></li></ul>

Return a iterator over the contents of the specified column families.

<a name="iterators-4"></a>

### iterators/4 ###

`iterators(DBHandle, CFHandles, ReadOpts, X4) -> any()`

Return keys iterator over the contents of the specified column families.

<a name="list_column_families-2"></a>

### list_column_families/2 ###

<pre><code>
list_column_families(Name, DBOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>Name = <a href="file.md#type-filename_all">file:filename_all()</a></code></li><li><code>DBOpts = <a href="#type-db_options">db_options()</a></code></li><li><code>Res = {ok, [string()]} | {error, any()}</code></li></ul>

List column families

<a name="open-2"></a>

### open/2 ###

<pre><code>
open(Name, DBOpts) -&gt; Result
</code></pre>

<ul class="definitions"><li><code>Name = <a href="file.md#type-filename_all">file:filename_all()</a></code></li><li><code>DBOpts = <a href="#type-db_options">db_options()</a></code></li><li><code>Result = {ok, <a href="#type-db_handle">db_handle()</a>} | {error, any()}</code></li></ul>

Open RocksDB with the defalut column family

<a name="open_with_cf-3"></a>

### open_with_cf/3 ###

<pre><code>
open_with_cf(Name, DBOpts, CFDescriptors) -&gt; {ok, <a href="#type-db_handle">db_handle()</a>, [<a href="#type-cf_handle">cf_handle()</a>]} | {error, any()}
</code></pre>

<ul class="definitions"><li><code>Name = <a href="file.md#type-filename_all">file:filename_all()</a></code></li><li><code>DBOpts = <a href="#type-db_options">db_options()</a></code></li><li><code>CFDescriptors = [#cf_descriptor{name = string(), options = <a href="#type-cf_options">cf_options()</a>}]</code></li></ul>

Open RocksDB with the specified column families

<a name="put-4"></a>

### put/4 ###

<pre><code>
put(DBHandle, Key, Value, WriteOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>Key = binary()</code></li><li><code>Value = binary()</code></li><li><code>WriteOpts = <a href="#type-write_options">write_options()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

Put a key/value pair into the default column family

<a name="put-5"></a>

### put/5 ###

<pre><code>
put(DBHandle, CFHandle, Key, Value, WriteOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>Key = binary()</code></li><li><code>Value = binary()</code></li><li><code>WriteOpts = <a href="#type-write_options">write_options()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

Put a key/value pair into the specified column family

<a name="release_snapshot-1"></a>

### release_snapshot/1 ###

<pre><code>
release_snapshot(SnapshotHandle::<a href="#type-snapshot_handle">snapshot_handle()</a>) -&gt; ok | {error, any()}
</code></pre>
<br />

release a snapshot

<a name="repair-2"></a>

### repair/2 ###

<pre><code>
repair(Name::<a href="file.md#type-filename_all">file:filename_all()</a>, DBOpts::<a href="#type-db_options">db_options()</a>) -&gt; ok | {error, any()}
</code></pre>
<br />

Try to repair as much of the contents of the database as possible.
Some data may be lost, so be careful when calling this function

<a name="snapshot-1"></a>

### snapshot/1 ###

<pre><code>
snapshot(DbHandle::<a href="#type-db_handle">db_handle()</a>) -&gt; {ok, <a href="#type-snapshot_handle">snapshot_handle()</a>} | {error, any()}
</code></pre>
<br />

return a database snapshot
Snapshots provide consistent read-only views over the entire state of the key-value store

<a name="stats-1"></a>

### stats/1 ###

<pre><code>
stats(DBHandle::<a href="#type-db_handle">db_handle()</a>) -&gt; {ok, any()} | {error, any()}
</code></pre>
<br />

Return the current stats of the default column family
Implemented by calling GetProperty with "rocksdb.stats"

<a name="stats-2"></a>

### stats/2 ###

<pre><code>
stats(DBHandle::<a href="#type-db_handle">db_handle()</a>, CFHandle::<a href="#type-cf_handle">cf_handle()</a>) -&gt; {ok, any()} | {error, any()}
</code></pre>
<br />

Return the current stats of the specified column family
Implemented by calling GetProperty with "rocksdb.stats"

<a name="write-3"></a>

### write/3 ###

<pre><code>
write(DBHandle, WriteActions, WriteOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>WriteActions = <a href="#type-write_actions">write_actions()</a></code></li><li><code>WriteOpts = <a href="#type-write_options">write_options()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

Apply the specified updates to the database.

