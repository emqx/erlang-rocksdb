

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




### <a name="type-backup_engine">backup_engine()</a> ###


__abstract datatype__: `backup_engine()`




### <a name="type-backup_info">backup_info()</a> ###


<pre><code>
backup_info() = #{id =&gt; non_neg_integer(), timestamp =&gt; non_neg_integer(), size =&gt; non_neg_integer(), number_files =&gt; non_neg_integer()}
</code></pre>




### <a name="type-batch_handle">batch_handle()</a> ###


__abstract datatype__: `batch_handle()`




### <a name="type-block_based_table_options">block_based_table_options()</a> ###


<pre><code>
block_based_table_options() = [{no_block_cache, boolean()} | {block_size, pos_integer()} | {block_cache, <a href="#type-cache_handle">cache_handle()</a>} | {block_cache_size, pos_integer()} | {bloom_filter_policy, BitsPerKey::pos_integer()} | {format_version, 0 | 1 | 2 | 3 | 4 | 5} | {cache_index_and_filter_blocks, boolean()}]
</code></pre>




### <a name="type-cache_handle">cache_handle()</a> ###


__abstract datatype__: `cache_handle()`




### <a name="type-cache_type">cache_type()</a> ###


<pre><code>
cache_type() = lru | clock
</code></pre>




### <a name="type-cf_descriptor">cf_descriptor()</a> ###


<pre><code>
cf_descriptor() = {string(), <a href="#type-cf_options">cf_options()</a>}
</code></pre>




### <a name="type-cf_handle">cf_handle()</a> ###


__abstract datatype__: `cf_handle()`




### <a name="type-cf_options">cf_options()</a> ###


<pre><code>
cf_options() = [{block_cache_size_mb_for_point_lookup, non_neg_integer()} | {memtable_memory_budget, pos_integer()} | {write_buffer_size, pos_integer()} | {max_write_buffer_number, pos_integer()} | {min_write_buffer_number_to_merge, pos_integer()} | {enable_blob_files, boolean()} | {min_blob_size, non_neg_integer()} | {blob_file_size, non_neg_integer()} | {blob_compression_type, <a href="#type-compression_type">compression_type()</a>} | {enable_blob_garbage_collection, boolean()} | {blob_garbage_collection_age_cutoff, float()} | {blob_garbage_collection_force_threshold, float()} | {blob_compaction_readahead_size, non_neg_integer()} | {blob_file_starting_level, non_neg_integer()} | {blob_cache, <a href="#type-cache_handle">cache_handle()</a>} | {prepopulate_blob_cache, <a href="#type-prepopulate_blob_cache">prepopulate_blob_cache()</a>} | {compression, <a href="#type-compression_type">compression_type()</a>} | {bottommost_compression, <a href="#type-compression_type">compression_type()</a>} | {compression_opts, <a href="#type-compression_opts">compression_opts()</a>} | {bottommost_compression_opts, <a href="#type-compression_opts">compression_opts()</a>} | {num_levels, pos_integer()} | {level0_file_num_compaction_trigger, integer()} | {level0_slowdown_writes_trigger, integer()} | {level0_stop_writes_trigger, integer()} | {target_file_size_base, pos_integer()} | {target_file_size_multiplier, pos_integer()} | {max_bytes_for_level_base, pos_integer()} | {max_bytes_for_level_multiplier, pos_integer()} | {max_compaction_bytes, pos_integer()} | {arena_block_size, integer()} | {disable_auto_compactions, boolean()} | {compaction_style, <a href="#type-compaction_style">compaction_style()</a>} | {compaction_pri, <a href="#type-compaction_pri">compaction_pri()</a>} | {filter_deletes, boolean()} | {max_sequential_skip_in_iterations, pos_integer()} | {inplace_update_support, boolean()} | {inplace_update_num_locks, pos_integer()} | {table_factory_block_cache_size, pos_integer()} | {in_memory_mode, boolean()} | {block_based_table_options, <a href="#type-block_based_table_options">block_based_table_options()</a>} | {level_compaction_dynamic_level_bytes, boolean()} | {optimize_filters_for_hits, boolean()} | {prefix_extractor, {fixed_prefix_transform, integer()} | {capped_prefix_transform, integer()}} | {merge_operator, <a href="#type-merge_operator">merge_operator()</a>}]
</code></pre>




### <a name="type-column_family">column_family()</a> ###


<pre><code>
column_family() = <a href="#type-cf_handle">cf_handle()</a> | default_column_family
</code></pre>




### <a name="type-compact_range_options">compact_range_options()</a> ###


<pre><code>
compact_range_options() = [{exclusive_manual_compaction, boolean()} | {change_level, boolean()} | {target_level, integer()} | {allow_write_stall, boolean()} | {max_subcompactions, non_neg_integer()}]
</code></pre>




### <a name="type-compaction_pri">compaction_pri()</a> ###


<pre><code>
compaction_pri() = compensated_size | oldest_largest_seq_first | oldest_smallest_seq_first
</code></pre>




### <a name="type-compaction_style">compaction_style()</a> ###


<pre><code>
compaction_style() = level | universal | fifo | none
</code></pre>




### <a name="type-compression_opts">compression_opts()</a> ###


<pre><code>
compression_opts() = [{enabled, boolean()} | {window_bits, pos_integer()} | {level, non_neg_integer()} | {strategy, integer()} | {max_dict_bytes, non_neg_integer()} | {zstd_max_train_bytes, non_neg_integer()}]
</code></pre>




### <a name="type-compression_type">compression_type()</a> ###


<pre><code>
compression_type() = snappy | zlib | bzip2 | lz4 | lz4h | zstd | none
</code></pre>




### <a name="type-db_handle">db_handle()</a> ###


__abstract datatype__: `db_handle()`




### <a name="type-db_options">db_options()</a> ###


<pre><code>
db_options() = [{env, <a href="#type-env">env()</a>} | {total_threads, pos_integer()} | {create_if_missing, boolean()} | {create_missing_column_families, boolean()} | {error_if_exists, boolean()} | {paranoid_checks, boolean()} | {max_open_files, integer()} | {max_total_wal_size, non_neg_integer()} | {use_fsync, boolean()} | {db_paths, [#db_path{path = <a href="http://www.erlang.org/edoc/doc/kernel/doc/file.html#type-filename_all">file:filename_all()</a>, target_size = non_neg_integer()}]} | {db_log_dir, <a href="http://www.erlang.org/edoc/doc/kernel/doc/file.html#type-filename_all">file:filename_all()</a>} | {wal_dir, <a href="http://www.erlang.org/edoc/doc/kernel/doc/file.html#type-filename_all">file:filename_all()</a>} | {delete_obsolete_files_period_micros, pos_integer()} | {max_background_jobs, pos_integer()} | {max_background_compactions, pos_integer()} | {max_background_flushes, pos_integer()} | {max_log_file_size, non_neg_integer()} | {log_file_time_to_roll, non_neg_integer()} | {keep_log_file_num, pos_integer()} | {max_manifest_file_size, pos_integer()} | {table_cache_numshardbits, pos_integer()} | {wal_ttl_seconds, non_neg_integer()} | {manual_wal_flush, boolean()} | {wal_size_limit_mb, non_neg_integer()} | {manifest_preallocation_size, pos_integer()} | {allow_mmap_reads, boolean()} | {allow_mmap_writes, boolean()} | {is_fd_close_on_exec, boolean()} | {stats_dump_period_sec, non_neg_integer()} | {advise_random_on_open, boolean()} | {access_hint, <a href="#type-access_hint">access_hint()</a>} | {compaction_readahead_size, non_neg_integer()} | {use_adaptive_mutex, boolean()} | {bytes_per_sync, non_neg_integer()} | {skip_stats_update_on_db_open, boolean()} | {wal_recovery_mode, <a href="#type-wal_recovery_mode">wal_recovery_mode()</a>} | {allow_concurrent_memtable_write, boolean()} | {enable_write_thread_adaptive_yield, boolean()} | {db_write_buffer_size, non_neg_integer()} | {in_memory, boolean()} | {rate_limiter, <a href="#type-rate_limiter_handle">rate_limiter_handle()</a>} | {sst_file_manager, <a href="#type-sst_file_manager">sst_file_manager()</a>} | {write_buffer_manager, <a href="#type-write_buffer_manager">write_buffer_manager()</a>} | {max_subcompactions, non_neg_integer()} | {atomic_flush, boolean()} | {use_direct_reads, boolean()} | {use_direct_io_for_flush_and_compaction, boolean()} | {enable_pipelined_write, boolean()} | {unordered_write, boolean()} | {two_write_queues, boolean()} | {statistics, <a href="#type-statistics_handle">statistics_handle()</a>}]
</code></pre>




### <a name="type-env">env()</a> ###


__abstract datatype__: `env()`




### <a name="type-env_handle">env_handle()</a> ###


__abstract datatype__: `env_handle()`




### <a name="type-env_priority">env_priority()</a> ###


<pre><code>
env_priority() = priority_high | priority_low
</code></pre>




### <a name="type-env_type">env_type()</a> ###


<pre><code>
env_type() = default | memenv
</code></pre>




### <a name="type-flush_options">flush_options()</a> ###


<pre><code>
flush_options() = [{wait, boolean()} | {allow_write_stall, boolean()}]
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
iterator_action() = first | last | next | prev | binary() | {seek, binary()} | {seek_for_prev, binary()}
</code></pre>




### <a name="type-itr_handle">itr_handle()</a> ###


__abstract datatype__: `itr_handle()`




### <a name="type-merge_operator">merge_operator()</a> ###


<pre><code>
merge_operator() = erlang_merge_operator | bitset_merge_operator | {bitset_merge_operator, non_neg_integer()} | counter_merge_operator
</code></pre>




### <a name="type-options">options()</a> ###


<pre><code>
options() = <a href="#type-db_options">db_options()</a> | <a href="#type-cf_options">cf_options()</a>
</code></pre>




### <a name="type-prepopulate_blob_cache">prepopulate_blob_cache()</a> ###


<pre><code>
prepopulate_blob_cache() = disable | flush_only
</code></pre>




### <a name="type-range">range()</a> ###


<pre><code>
range() = {Start::binary(), Limit::binary()}
</code></pre>




### <a name="type-rate_limiter_handle">rate_limiter_handle()</a> ###


__abstract datatype__: `rate_limiter_handle()`




### <a name="type-read_options">read_options()</a> ###


<pre><code>
read_options() = [{read_tier, <a href="#type-read_tier">read_tier()</a>} | {verify_checksums, boolean()} | {fill_cache, boolean()} | {iterate_upper_bound, binary()} | {iterate_lower_bound, binary()} | {tailing, boolean()} | {total_order_seek, boolean()} | {prefix_same_as_start, boolean()} | {snapshot, <a href="#type-snapshot_handle">snapshot_handle()</a>}]
</code></pre>




### <a name="type-read_tier">read_tier()</a> ###


<pre><code>
read_tier() = read_all_tier | block_cache_tier | persisted_tier | memtable_tier
</code></pre>




### <a name="type-size_approximation_flag">size_approximation_flag()</a> ###


<pre><code>
size_approximation_flag() = none | include_memtables | include_files | include_both
</code></pre>




### <a name="type-snapshot_handle">snapshot_handle()</a> ###


__abstract datatype__: `snapshot_handle()`




### <a name="type-sst_file_manager">sst_file_manager()</a> ###


__abstract datatype__: `sst_file_manager()`




### <a name="type-statistics_handle">statistics_handle()</a> ###


__abstract datatype__: `statistics_handle()`




### <a name="type-stats_level">stats_level()</a> ###


<pre><code>
stats_level() = stats_disable_all | stats_except_tickers | stats_except_histogram_or_timers | stats_except_timers | stats_except_detailed_timers | stats_except_time_for_mutex | stats_all
</code></pre>




### <a name="type-transaction_handle">transaction_handle()</a> ###


__abstract datatype__: `transaction_handle()`




### <a name="type-wal_recovery_mode">wal_recovery_mode()</a> ###


<pre><code>
wal_recovery_mode() = tolerate_corrupted_tail_records | absolute_consistency | point_in_time_recovery | skip_any_corrupted_records
</code></pre>




### <a name="type-write_actions">write_actions()</a> ###


<pre><code>
write_actions() = [{put, Key::binary(), Value::binary()} | {put, ColumnFamilyHandle::<a href="#type-cf_handle">cf_handle()</a>, Key::binary(), Value::binary()} | {delete, Key::binary()} | {delete, ColumnFamilyHandle::<a href="#type-cf_handle">cf_handle()</a>, Key::binary()} | {single_delete, Key::binary()} | {single_delete, ColumnFamilyHandle::<a href="#type-cf_handle">cf_handle()</a>, Key::binary()} | clear]
</code></pre>




### <a name="type-write_buffer_manager">write_buffer_manager()</a> ###


__abstract datatype__: `write_buffer_manager()`




### <a name="type-write_options">write_options()</a> ###


<pre><code>
write_options() = [{sync, boolean()} | {disable_wal, boolean()} | {ignore_missing_column_families, boolean()} | {no_slowdown, boolean()} | {low_pri, boolean()}]
</code></pre>

<a name="index"></a>

## Function Index ##


<table width="100%" border="1" cellspacing="0" cellpadding="2" summary="function index"><tr><td valign="top"><a href="#batch-0">batch/0</a></td><td>create a new batch in memory.</td></tr><tr><td valign="top"><a href="#batch_clear-1">batch_clear/1</a></td><td>reset the batch, clear all operations.</td></tr><tr><td valign="top"><a href="#batch_count-1">batch_count/1</a></td><td>return the number of operations in the batch.</td></tr><tr><td valign="top"><a href="#batch_data_size-1">batch_data_size/1</a></td><td>Retrieve data size of the batch.</td></tr><tr><td valign="top"><a href="#batch_delete-2">batch_delete/2</a></td><td>batch implementation of delete operation to the batch.</td></tr><tr><td valign="top"><a href="#batch_delete-3">batch_delete/3</a></td><td>like <code>batch_delete/2</code> but apply the operation to a column family.</td></tr><tr><td valign="top"><a href="#batch_merge-3">batch_merge/3</a></td><td>add a merge operation to the batch.</td></tr><tr><td valign="top"><a href="#batch_merge-4">batch_merge/4</a></td><td>like <code>batch_mege/3</code> but apply the operation to a column family.</td></tr><tr><td valign="top"><a href="#batch_put-3">batch_put/3</a></td><td>add a put operation to the batch.</td></tr><tr><td valign="top"><a href="#batch_put-4">batch_put/4</a></td><td>like <code>batch_put/3</code> but apply the operation to a column family.</td></tr><tr><td valign="top"><a href="#batch_rollback-1">batch_rollback/1</a></td><td>rollback the operations to the latest checkpoint.</td></tr><tr><td valign="top"><a href="#batch_savepoint-1">batch_savepoint/1</a></td><td>store a checkpoint in the batch to which you can rollback later.</td></tr><tr><td valign="top"><a href="#batch_single_delete-2">batch_single_delete/2</a></td><td>batch implementation of single_delete operation to the batch.</td></tr><tr><td valign="top"><a href="#batch_single_delete-3">batch_single_delete/3</a></td><td>like <code>batch_single_delete/2</code> but apply the operation to a column family.</td></tr><tr><td valign="top"><a href="#batch_tolist-1">batch_tolist/1</a></td><td>return all the operation sin the batch as a list of operations.</td></tr><tr><td valign="top"><a href="#cache_info-1">cache_info/1</a></td><td>return informations of a cache as a list of tuples.</td></tr><tr><td valign="top"><a href="#cache_info-2">cache_info/2</a></td><td>return the information associated with Item for cache Cache.</td></tr><tr><td valign="top"><a href="#checkpoint-2">checkpoint/2</a></td><td>take a snapshot of a running RocksDB database in a separate directory
http://rocksdb.org/blog/2609/use-checkpoints-for-efficient-snapshots/.</td></tr><tr><td valign="top"><a href="#close-1">close/1</a></td><td>Close RocksDB.</td></tr><tr><td valign="top"><a href="#close_backup_engine-1">close_backup_engine/1</a></td><td>stop and close the backup engine
note: experimental for testing only.</td></tr><tr><td valign="top"><a href="#close_updates_iterator-1">close_updates_iterator/1</a></td><td></td></tr><tr><td valign="top"><a href="#compact_range-4">compact_range/4</a></td><td>Compact the underlying storage for the key range [*begin,*end].</td></tr><tr><td valign="top"><a href="#compact_range-5">compact_range/5</a></td><td> Compact the underlying storage for the key range ["BeginKey", "EndKey").</td></tr><tr><td valign="top"><a href="#count-1">count/1</a></td><td>Return the approximate number of keys in the default column family.</td></tr><tr><td valign="top"><a href="#count-2">count/2</a></td><td>
Return the approximate number of keys in the specified column family.</td></tr><tr><td valign="top"><a href="#create_column_family-3">create_column_family/3</a></td><td>Create a new column family.</td></tr><tr><td valign="top"><a href="#create_new_backup-2">create_new_backup/2</a></td><td>Captures the state of the database in the latest backup.</td></tr><tr><td valign="top"><a href="#default_env-0">default_env/0</a></td><td></td></tr><tr><td valign="top"><a href="#delete-3">delete/3</a></td><td>Delete a key/value pair in the default column family.</td></tr><tr><td valign="top"><a href="#delete-4">delete/4</a></td><td>Delete a key/value pair in the specified column family.</td></tr><tr><td valign="top"><a href="#delete_backup-2">delete_backup/2</a></td><td>deletes a specific backup.</td></tr><tr><td valign="top"><a href="#delete_range-4">delete_range/4</a></td><td>Removes the database entries in the range ["BeginKey", "EndKey"), i.e.,
including "BeginKey" and excluding "EndKey".</td></tr><tr><td valign="top"><a href="#delete_range-5">delete_range/5</a></td><td>Removes the database entries in the range ["BeginKey", "EndKey").</td></tr><tr><td valign="top"><a href="#destroy-2">destroy/2</a></td><td>Destroy the contents of the specified database.</td></tr><tr><td valign="top"><a href="#destroy_column_family-1">destroy_column_family/1</a></td><td></td></tr><tr><td valign="top"><a href="#destroy_column_family-2">destroy_column_family/2</a></td><td>Destroy a column family.</td></tr><tr><td valign="top"><a href="#destroy_env-1">destroy_env/1</a></td><td>destroy an environment.</td></tr><tr><td valign="top"><a href="#drop_column_family-1">drop_column_family/1</a></td><td></td></tr><tr><td valign="top"><a href="#drop_column_family-2">drop_column_family/2</a></td><td>Drop a column family.</td></tr><tr><td valign="top"><a href="#flush-2">flush/2</a></td><td>Flush all mem-table data.</td></tr><tr><td valign="top"><a href="#flush-3">flush/3</a></td><td>Flush all mem-table data for a column family.</td></tr><tr><td valign="top"><a href="#fold-4">fold/4</a></td><td>Calls Fun(Elem, AccIn) on successive elements in the default column family
starting with AccIn == Acc0.</td></tr><tr><td valign="top"><a href="#fold-5">fold/5</a></td><td>Calls Fun(Elem, AccIn) on successive elements in the specified column family
Other specs are same with fold/4.</td></tr><tr><td valign="top"><a href="#fold_keys-4">fold_keys/4</a></td><td>Calls Fun(Elem, AccIn) on successive elements in the default column family
starting with AccIn == Acc0.</td></tr><tr><td valign="top"><a href="#fold_keys-5">fold_keys/5</a></td><td>Calls Fun(Elem, AccIn) on successive elements in the specified column family
Other specs are same with fold_keys/4.</td></tr><tr><td valign="top"><a href="#gc_backup_engine-1">gc_backup_engine/1</a></td><td> Will delete all the files we don't need anymore
It will do the full scan of the files/ directory and delete all the
files that are not referenced.</td></tr><tr><td valign="top"><a href="#get-3">get/3</a></td><td>Retrieve a key/value pair in the default column family.</td></tr><tr><td valign="top"><a href="#get-4">get/4</a></td><td>Retrieve a key/value pair in the specified column family.</td></tr><tr><td valign="top"><a href="#get_approximate_memtable_stats-3">get_approximate_memtable_stats/3</a></td><td>The method is similar to GetApproximateSizes, except it
returns approximate number of records in memtables.</td></tr><tr><td valign="top"><a href="#get_approximate_memtable_stats-4">get_approximate_memtable_stats/4</a></td><td></td></tr><tr><td valign="top"><a href="#get_approximate_sizes-3">get_approximate_sizes/3</a></td><td>For each i in [0,n-1], store in "Sizes[i]", the approximate
file system space used by keys in "[range[i].start ..</td></tr><tr><td valign="top"><a href="#get_approximate_sizes-4">get_approximate_sizes/4</a></td><td></td></tr><tr><td valign="top"><a href="#get_backup_info-1">get_backup_info/1</a></td><td>Returns info about backups in backup_info.</td></tr><tr><td valign="top"><a href="#get_capacity-1">get_capacity/1</a></td><td></td></tr><tr><td valign="top"><a href="#get_latest_sequence_number-1">get_latest_sequence_number/1</a></td><td>gThe sequence number of the most recent transaction.</td></tr><tr><td valign="top"><a href="#get_pinned_usage-1">get_pinned_usage/1</a></td><td></td></tr><tr><td valign="top"><a href="#get_property-2">get_property/2</a></td><td>Return the RocksDB internal status of the default column family specified at Property.</td></tr><tr><td valign="top"><a href="#get_property-3">get_property/3</a></td><td>Return the RocksDB internal status of the specified column family specified at Property.</td></tr><tr><td valign="top"><a href="#get_snapshot_sequence-1">get_snapshot_sequence/1</a></td><td>returns Snapshot's sequence number.</td></tr><tr><td valign="top"><a href="#get_usage-1">get_usage/1</a></td><td></td></tr><tr><td valign="top"><a href="#is_empty-1">is_empty/1</a></td><td>is the database empty.</td></tr><tr><td valign="top"><a href="#iterator-2">iterator/2</a></td><td>Return a iterator over the contents of the database.</td></tr><tr><td valign="top"><a href="#iterator-3">iterator/3</a></td><td>Return a iterator over the contents of the database.</td></tr><tr><td valign="top"><a href="#iterator_close-1">iterator_close/1</a></td><td>
Close a iterator.</td></tr><tr><td valign="top"><a href="#iterator_move-2">iterator_move/2</a></td><td>
Move to the specified place.</td></tr><tr><td valign="top"><a href="#iterator_refresh-1">iterator_refresh/1</a></td><td>
Refresh iterator.</td></tr><tr><td valign="top"><a href="#iterators-3">iterators/3</a></td><td>
Return a iterator over the contents of the specified column family.</td></tr><tr><td valign="top"><a href="#list_column_families-2">list_column_families/2</a></td><td>List column families.</td></tr><tr><td valign="top"><a href="#mem_env-0">mem_env/0</a></td><td></td></tr><tr><td valign="top"><a href="#merge-4">merge/4</a></td><td>Merge a key/value pair into the default column family.</td></tr><tr><td valign="top"><a href="#merge-5">merge/5</a></td><td>Merge a key/value pair into the specified column family.</td></tr><tr><td valign="top"><a href="#new_cache-2">new_cache/2</a></td><td>// Create a new cache.</td></tr><tr><td valign="top"><a href="#new_clock_cache-1">new_clock_cache/1</a></td><td></td></tr><tr><td valign="top"><a href="#new_env-0">new_env/0</a></td><td>return a default db environment.</td></tr><tr><td valign="top"><a href="#new_env-1">new_env/1</a></td><td>return a db environment.</td></tr><tr><td valign="top"><a href="#new_lru_cache-1">new_lru_cache/1</a></td><td></td></tr><tr><td valign="top"><a href="#new_rate_limiter-2">new_rate_limiter/2</a></td><td>create new Limiter.</td></tr><tr><td valign="top"><a href="#new_sst_file_manager-1">new_sst_file_manager/1</a></td><td>create new SstFileManager with the default options:
RateBytesPerSec = 0, MaxTrashDbRatio = 0.25, BytesMaxDeleteChunk = 64 * 1024 * 1024.</td></tr><tr><td valign="top"><a href="#new_sst_file_manager-2">new_sst_file_manager/2</a></td><td>create new SstFileManager that can be shared among multiple RocksDB
instances to track SST file and control there deletion rate.</td></tr><tr><td valign="top"><a href="#new_statistics-0">new_statistics/0</a></td><td></td></tr><tr><td valign="top"><a href="#new_write_buffer_manager-1">new_write_buffer_manager/1</a></td><td> create a new WriteBufferManager.</td></tr><tr><td valign="top"><a href="#new_write_buffer_manager-2">new_write_buffer_manager/2</a></td><td> create a new WriteBufferManager.</td></tr><tr><td valign="top"><a href="#next_binary_update-1">next_binary_update/1</a></td><td></td></tr><tr><td valign="top"><a href="#next_update-1">next_update/1</a></td><td></td></tr><tr><td valign="top"><a href="#open-2">open/2</a></td><td>Open RocksDB with the defalut column family.</td></tr><tr><td valign="top"><a href="#open-3">open/3</a></td><td>Open RocksDB with the specified column families.</td></tr><tr><td valign="top"><a href="#open_backup_engine-1">open_backup_engine/1</a></td><td>open a new backup engine for creating new backups.</td></tr><tr><td valign="top"><a href="#open_optimistic_transaction_db-2">open_optimistic_transaction_db/2</a></td><td></td></tr><tr><td valign="top"><a href="#open_optimistic_transaction_db-3">open_optimistic_transaction_db/3</a></td><td></td></tr><tr><td valign="top"><a href="#open_readonly-2">open_readonly/2</a></td><td></td></tr><tr><td valign="top"><a href="#open_readonly-3">open_readonly/3</a></td><td>Open read-only RocksDB with the specified column families.</td></tr><tr><td valign="top"><a href="#open_with_cf-3">open_with_cf/3</a></td><td></td></tr><tr><td valign="top"><a href="#open_with_cf_readonly-3">open_with_cf_readonly/3</a></td><td></td></tr><tr><td valign="top"><a href="#open_with_ttl-4">open_with_ttl/4</a></td><td>Open RocksDB with TTL support
This API should be used to open the db when key-values inserted are
meant to be removed from the db in a non-strict <code>TTL</code> amount of time
Therefore, this guarantees that key-values inserted will remain in the
db for >= TTL amount of time and the db will make efforts to remove the
key-values as soon as possible after ttl seconds of their insertion.</td></tr><tr><td valign="top"><a href="#purge_old_backup-2">purge_old_backup/2</a></td><td>deletes old backups, keeping latest num_backups_to_keep alive.</td></tr><tr><td valign="top"><a href="#put-4">put/4</a></td><td>Put a key/value pair into the default column family.</td></tr><tr><td valign="top"><a href="#put-5">put/5</a></td><td>Put a key/value pair into the specified column family.</td></tr><tr><td valign="top"><a href="#release_batch-1">release_batch/1</a></td><td></td></tr><tr><td valign="top"><a href="#release_cache-1">release_cache/1</a></td><td>release the cache.</td></tr><tr><td valign="top"><a href="#release_rate_limiter-1">release_rate_limiter/1</a></td><td>release the limiter.</td></tr><tr><td valign="top"><a href="#release_snapshot-1">release_snapshot/1</a></td><td>release a snapshot.</td></tr><tr><td valign="top"><a href="#release_sst_file_manager-1">release_sst_file_manager/1</a></td><td>release the SstFileManager.</td></tr><tr><td valign="top"><a href="#release_statistics-1">release_statistics/1</a></td><td>release the Statistics Handle.</td></tr><tr><td valign="top"><a href="#release_transaction-1">release_transaction/1</a></td><td>release a transaction.</td></tr><tr><td valign="top"><a href="#release_write_buffer_manager-1">release_write_buffer_manager/1</a></td><td></td></tr><tr><td valign="top"><a href="#repair-2">repair/2</a></td><td>Try to repair as much of the contents of the database as possible.</td></tr><tr><td valign="top"><a href="#restore_db_from_backup-3">restore_db_from_backup/3</a></td><td>restore from backup with backup_id.</td></tr><tr><td valign="top"><a href="#restore_db_from_backup-4">restore_db_from_backup/4</a></td><td>restore from backup with backup_id.</td></tr><tr><td valign="top"><a href="#restore_db_from_latest_backup-2">restore_db_from_latest_backup/2</a></td><td>restore from the latest backup.</td></tr><tr><td valign="top"><a href="#restore_db_from_latest_backup-3">restore_db_from_latest_backup/3</a></td><td>restore from the latest backup.</td></tr><tr><td valign="top"><a href="#set_capacity-2">set_capacity/2</a></td><td>sets the maximum configured capacity of the cache.</td></tr><tr><td valign="top"><a href="#set_db_background_threads-2">set_db_background_threads/2</a></td><td>set background threads of a database.</td></tr><tr><td valign="top"><a href="#set_db_background_threads-3">set_db_background_threads/3</a></td><td>set database background threads of low and high prioriry threads pool of an environment
Flush threads are in the HIGH priority pool, while compaction threads are in the
LOW priority pool.</td></tr><tr><td valign="top"><a href="#set_env_background_threads-2">set_env_background_threads/2</a></td><td>set background threads of an environment.</td></tr><tr><td valign="top"><a href="#set_env_background_threads-3">set_env_background_threads/3</a></td><td>set background threads of low and high prioriry threads pool of an environment
Flush threads are in the HIGH priority pool, while compaction threads are in the
LOW priority pool.</td></tr><tr><td valign="top"><a href="#set_stats_level-2">set_stats_level/2</a></td><td></td></tr><tr><td valign="top"><a href="#set_strict_capacity_limit-2">set_strict_capacity_limit/2</a></td><td>sets strict_capacity_limit flag of the cache.</td></tr><tr><td valign="top"><a href="#single_delete-3">single_delete/3</a></td><td>Remove the database entry for "key".</td></tr><tr><td valign="top"><a href="#single_delete-4">single_delete/4</a></td><td>like <code>single_delete/3</code> but on the specified column family.</td></tr><tr><td valign="top"><a href="#snapshot-1">snapshot/1</a></td><td>return a database snapshot
Snapshots provide consistent read-only views over the entire state of the key-value store.</td></tr><tr><td valign="top"><a href="#sst_file_manager_flag-3">sst_file_manager_flag/3</a></td><td>set certains flags for the SST file manager
* <code>max_allowed_space_usage</code>: Update the maximum allowed space that should be used by RocksDB, if
the total size of the SST files exceeds MaxAllowedSpace, writes to
RocksDB will fail.</td></tr><tr><td valign="top"><a href="#sst_file_manager_info-1">sst_file_manager_info/1</a></td><td>return informations of a Sst File Manager as a list of tuples.</td></tr><tr><td valign="top"><a href="#sst_file_manager_info-2">sst_file_manager_info/2</a></td><td>return the information associated with Item for an SST File Manager SstFileManager.</td></tr><tr><td valign="top"><a href="#statistics_info-1">statistics_info/1</a></td><td></td></tr><tr><td valign="top"><a href="#stats-1">stats/1</a></td><td>Return the current stats of the default column family
Implemented by calling GetProperty with "rocksdb.stats".</td></tr><tr><td valign="top"><a href="#stats-2">stats/2</a></td><td>Return the current stats of the specified column family
Implemented by calling GetProperty with "rocksdb.stats".</td></tr><tr><td valign="top"><a href="#stop_backup-1">stop_backup/1</a></td><td></td></tr><tr><td valign="top"><a href="#sync_wal-1">sync_wal/1</a></td><td> Sync the wal.</td></tr><tr><td valign="top"><a href="#tlog_iterator-2">tlog_iterator/2</a></td><td>create a new iterator to retrive ethe transaction log since a sequce.</td></tr><tr><td valign="top"><a href="#tlog_iterator_close-1">tlog_iterator_close/1</a></td><td>close the transaction log.</td></tr><tr><td valign="top"><a href="#tlog_next_binary_update-1">tlog_next_binary_update/1</a></td><td>go to the last update as a binary in the transaction log, can be ussed with the write_binary_update function.</td></tr><tr><td valign="top"><a href="#tlog_next_update-1">tlog_next_update/1</a></td><td>like <code>tlog_nex_binary_update/1</code> but also return the batch as a list of operations.</td></tr><tr><td valign="top"><a href="#transaction-2">transaction/2</a></td><td>create a new transaction
When opened as a Transaction or Optimistic Transaction db,
a user can both read and write to a transaction without committing
anything to the disk until they decide to do so.</td></tr><tr><td valign="top"><a href="#transaction_commit-1">transaction_commit/1</a></td><td>commit a transaction to disk atomically (?).</td></tr><tr><td valign="top"><a href="#transaction_delete-2">transaction_delete/2</a></td><td>transaction implementation of delete operation to the transaction.</td></tr><tr><td valign="top"><a href="#transaction_delete-3">transaction_delete/3</a></td><td>like <code>transaction_delete/2</code> but apply the operation to a column family.</td></tr><tr><td valign="top"><a href="#transaction_get-3">transaction_get/3</a></td><td>do a get operation on the contents of the transaction.</td></tr><tr><td valign="top"><a href="#transaction_get-4">transaction_get/4</a></td><td>like <code>transaction_get/3</code> but apply the operation to a column family.</td></tr><tr><td valign="top"><a href="#transaction_iterator-2">transaction_iterator/2</a></td><td>Return a iterator over the contents of the database and
uncommited writes and deletes in the current transaction.</td></tr><tr><td valign="top"><a href="#transaction_iterator-3">transaction_iterator/3</a></td><td>Return a iterator over the contents of the database and
uncommited writes and deletes in the current transaction.</td></tr><tr><td valign="top"><a href="#transaction_put-3">transaction_put/3</a></td><td>add a put operation to the transaction.</td></tr><tr><td valign="top"><a href="#transaction_put-4">transaction_put/4</a></td><td>like <code>transaction_put/3</code> but apply the operation to a column family.</td></tr><tr><td valign="top"><a href="#transaction_rollback-1">transaction_rollback/1</a></td><td>rollback a transaction to disk atomically (?).</td></tr><tr><td valign="top"><a href="#updates_iterator-2">updates_iterator/2</a></td><td></td></tr><tr><td valign="top"><a href="#verify_backup-2">verify_backup/2</a></td><td>checks that each file exists and that the size of the file matches
our expectations.</td></tr><tr><td valign="top"><a href="#write-3">write/3</a></td><td>Apply the specified updates to the database.</td></tr><tr><td valign="top"><a href="#write_batch-3">write_batch/3</a></td><td>write the batch to the database.</td></tr><tr><td valign="top"><a href="#write_binary_update-3">write_binary_update/3</a></td><td>apply a set of operation coming from a transaction log to another database.</td></tr><tr><td valign="top"><a href="#write_buffer_manager_info-1">write_buffer_manager_info/1</a></td><td>return informations of a Write Buffer Manager as a list of tuples.</td></tr><tr><td valign="top"><a href="#write_buffer_manager_info-2">write_buffer_manager_info/2</a></td><td>return the information associated with Item for a Write Buffer Manager.</td></tr></table>


<a name="functions"></a>

## Function Details ##

<a name="batch-0"></a>

### batch/0 ###

<pre><code>
batch() -&gt; {ok, Batch::<a href="#type-batch_handle">batch_handle()</a>}
</code></pre>
<br />

create a new batch in memory. A batch is a nif resource attached to the current process. Pay attention when you
share it with other processes as it may not been released. To force its release you will need to use the close_batch
function.

<a name="batch_clear-1"></a>

### batch_clear/1 ###

<pre><code>
batch_clear(Batch::<a href="#type-batch_handle">batch_handle()</a>) -&gt; ok
</code></pre>
<br />

reset the batch, clear all operations.

<a name="batch_count-1"></a>

### batch_count/1 ###

<pre><code>
batch_count(_Batch::<a href="#type-batch_handle">batch_handle()</a>) -&gt; Count::non_neg_integer()
</code></pre>
<br />

return the number of operations in the batch

<a name="batch_data_size-1"></a>

### batch_data_size/1 ###

<pre><code>
batch_data_size(_Batch::<a href="#type-batch_handle">batch_handle()</a>) -&gt; BatchSize::non_neg_integer()
</code></pre>
<br />

Retrieve data size of the batch.

<a name="batch_delete-2"></a>

### batch_delete/2 ###

<pre><code>
batch_delete(Batch::<a href="#type-batch_handle">batch_handle()</a>, Key::binary()) -&gt; ok
</code></pre>
<br />

batch implementation of delete operation to the batch

<a name="batch_delete-3"></a>

### batch_delete/3 ###

<pre><code>
batch_delete(Batch::<a href="#type-batch_handle">batch_handle()</a>, ColumnFamily::<a href="#type-cf_handle">cf_handle()</a>, Key::binary()) -&gt; ok
</code></pre>
<br />

like `batch_delete/2` but apply the operation to a column family

<a name="batch_merge-3"></a>

### batch_merge/3 ###

<pre><code>
batch_merge(Batch::<a href="#type-batch_handle">batch_handle()</a>, Key::binary(), Value::binary()) -&gt; ok
</code></pre>
<br />

add a merge operation to the batch

<a name="batch_merge-4"></a>

### batch_merge/4 ###

<pre><code>
batch_merge(Batch::<a href="#type-batch_handle">batch_handle()</a>, ColumnFamily::<a href="#type-cf_handle">cf_handle()</a>, Key::binary(), Value::binary()) -&gt; ok
</code></pre>
<br />

like `batch_mege/3` but apply the operation to a column family

<a name="batch_put-3"></a>

### batch_put/3 ###

<pre><code>
batch_put(Batch::<a href="#type-batch_handle">batch_handle()</a>, Key::binary(), Value::binary()) -&gt; ok
</code></pre>
<br />

add a put operation to the batch

<a name="batch_put-4"></a>

### batch_put/4 ###

<pre><code>
batch_put(Batch::<a href="#type-batch_handle">batch_handle()</a>, ColumnFamily::<a href="#type-cf_handle">cf_handle()</a>, Key::binary(), Value::binary()) -&gt; ok
</code></pre>
<br />

like `batch_put/3` but apply the operation to a column family

<a name="batch_rollback-1"></a>

### batch_rollback/1 ###

<pre><code>
batch_rollback(Batch::<a href="#type-batch_handle">batch_handle()</a>) -&gt; ok
</code></pre>
<br />

rollback the operations to the latest checkpoint

<a name="batch_savepoint-1"></a>

### batch_savepoint/1 ###

<pre><code>
batch_savepoint(Batch::<a href="#type-batch_handle">batch_handle()</a>) -&gt; ok
</code></pre>
<br />

store a checkpoint in the batch to which you can rollback later

<a name="batch_single_delete-2"></a>

### batch_single_delete/2 ###

<pre><code>
batch_single_delete(Batch::<a href="#type-batch_handle">batch_handle()</a>, Key::binary()) -&gt; ok
</code></pre>
<br />

batch implementation of single_delete operation to the batch

<a name="batch_single_delete-3"></a>

### batch_single_delete/3 ###

<pre><code>
batch_single_delete(Batch::<a href="#type-batch_handle">batch_handle()</a>, ColumnFamily::<a href="#type-cf_handle">cf_handle()</a>, Key::binary()) -&gt; ok
</code></pre>
<br />

like `batch_single_delete/2` but apply the operation to a column family

<a name="batch_tolist-1"></a>

### batch_tolist/1 ###

<pre><code>
batch_tolist(Batch::<a href="#type-batch_handle">batch_handle()</a>) -&gt; Ops::<a href="#type-write_actions">write_actions()</a>
</code></pre>
<br />

return all the operation sin the batch as a list of operations

<a name="cache_info-1"></a>

### cache_info/1 ###

<pre><code>
cache_info(Cache) -&gt; InfoList
</code></pre>

<ul class="definitions"><li><code>Cache = <a href="#type-cache_handle">cache_handle()</a></code></li><li><code>InfoList = [InfoTuple]</code></li><li><code>InfoTuple = {capacity, non_neg_integer()} | {strict_capacity, boolean()} | {usage, non_neg_integer()} | {pinned_usage, non_neg_integer()}</code></li></ul>

return informations of a cache as a list of tuples.
* `{capacity, integer >=0}`
the maximum configured capacity of the cache.
* `{strict_capacity, boolean}`
the flag whether to return error on insertion when cache reaches its full capacity.
* `{usage, integer >=0}`
the memory size for the entries residing in the cache.
* `{pinned_usage, integer >= 0}`
the memory size for the entries in use by the system

<a name="cache_info-2"></a>

### cache_info/2 ###

<pre><code>
cache_info(Cache, Item) -&gt; Value
</code></pre>

<ul class="definitions"><li><code>Cache = <a href="#type-cache_handle">cache_handle()</a></code></li><li><code>Item = capacity | strict_capacity | usage | pinned_usage</code></li><li><code>Value = term()</code></li></ul>

return the information associated with Item for cache Cache

<a name="checkpoint-2"></a>

### checkpoint/2 ###

<pre><code>
checkpoint(DbHandle::<a href="#type-db_handle">db_handle()</a>, Path::<a href="http://www.erlang.org/edoc/doc/kernel/doc/file.html#type-filename_all">file:filename_all()</a>) -&gt; ok | {error, any()}
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

<a name="close_backup_engine-1"></a>

### close_backup_engine/1 ###

<pre><code>
close_backup_engine(BackupEngine::<a href="#type-backup_engine">backup_engine()</a>) -&gt; ok
</code></pre>
<br />

stop and close the backup engine
note: experimental for testing only

<a name="close_updates_iterator-1"></a>

### close_updates_iterator/1 ###

`close_updates_iterator(Itr) -> any()`

<a name="compact_range-4"></a>

### compact_range/4 ###

<pre><code>
compact_range(DBHandle, BeginKey, EndKey, CompactRangeOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>BeginKey = binary() | undefined</code></li><li><code>EndKey = binary() | undefined</code></li><li><code>CompactRangeOpts = <a href="#type-compact_range_options">compact_range_options()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

Compact the underlying storage for the key range [*begin,*end].
The actual compaction interval might be superset of [*begin, *end].
In particular, deleted and overwritten versions are discarded,
and the data is rearranged to reduce the cost of operations
needed to access the data.  This operation should typically only
be invoked by users who understand the underlying implementation.

"begin==undefined" is treated as a key before all keys in the database.
"end==undefined" is treated as a key after all keys in the database.
Therefore the following call will compact the entire database:
rocksdb::compact_range(Options, undefined, undefined);
Note that after the entire database is compacted, all data are pushed
down to the last level containing any data. If the total data size after
compaction is reduced, that level might not be appropriate for hosting all
the files. In this case, client could set options.change_level to true, to
move the files back to the minimum level capable of holding the data set
or a given level (specified by non-negative target_level).

<a name="compact_range-5"></a>

### compact_range/5 ###

<pre><code>
compact_range(DBHandle, CFHandle, BeginKey, EndKey, CompactRangeOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>BeginKey = binary() | undefined</code></li><li><code>EndKey = binary() | undefined</code></li><li><code>CompactRangeOpts = <a href="#type-compact_range_options">compact_range_options()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

Compact the underlying storage for the key range ["BeginKey", "EndKey").
like `compact_range/3` but for a column family

<a name="count-1"></a>

### count/1 ###

<pre><code>
count(DBHandle::<a href="#type-db_handle">db_handle()</a>) -&gt; non_neg_integer() | {error, any()}
</code></pre>
<br />

Return the approximate number of keys in the default column family.
Implemented by calling GetIntProperty with "rocksdb.estimate-num-keys"

this function is deprecated and will be removed in next major release.

<a name="count-2"></a>

### count/2 ###

<pre><code>
count(DBHandle::<a href="#type-db_handle">db_handle()</a>, CFHandle::<a href="#type-cf_handle">cf_handle()</a>) -&gt; non_neg_integer() | {error, any()}
</code></pre>
<br />

Return the approximate number of keys in the specified column family.

this function is deprecated and will be removed in next major release.

<a name="create_column_family-3"></a>

### create_column_family/3 ###

<pre><code>
create_column_family(DBHandle, Name, CFOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>Name = string()</code></li><li><code>CFOpts = <a href="#type-cf_options">cf_options()</a></code></li><li><code>Res = {ok, <a href="#type-cf_handle">cf_handle()</a>} | {error, any()}</code></li></ul>

Create a new column family

<a name="create_new_backup-2"></a>

### create_new_backup/2 ###

<pre><code>
create_new_backup(BackupEngine::<a href="#type-backup_engine">backup_engine()</a>, Db::<a href="#type-db_handle">db_handle()</a>) -&gt; ok | {error, term()}
</code></pre>
<br />

Captures the state of the database in the latest backup

<a name="default_env-0"></a>

### default_env/0 ###

`default_env() -> any()`

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

<a name="delete_backup-2"></a>

### delete_backup/2 ###

<pre><code>
delete_backup(BackupEngine::<a href="#type-backup_engine">backup_engine()</a>, BackupId::non_neg_integer()) -&gt; ok | {error, any()}
</code></pre>
<br />

deletes a specific backup

<a name="delete_range-4"></a>

### delete_range/4 ###

<pre><code>
delete_range(DBHandle, BeginKey, EndKey, WriteOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>BeginKey = binary()</code></li><li><code>EndKey = binary()</code></li><li><code>WriteOpts = <a href="#type-write_options">write_options()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

Removes the database entries in the range ["BeginKey", "EndKey"), i.e.,
including "BeginKey" and excluding "EndKey". Returns OK on success, and
a non-OK status on error. It is not an error if no keys exist in the range
["BeginKey", "EndKey").

This feature is currently an experimental performance optimization for
deleting very large ranges of contiguous keys. Invoking it many times or on
small ranges may severely degrade read performance; in particular, the
resulting performance can be worse than calling Delete() for each key in
the range. Note also the degraded read performance affects keys outside the
deleted ranges, and affects database operations involving scans, like flush
and compaction.

Consider setting ReadOptions::ignore_range_deletions = true to speed
up reads for key(s) that are known to be unaffected by range deletions.

<a name="delete_range-5"></a>

### delete_range/5 ###

<pre><code>
delete_range(DBHandle, CFHandle, BeginKey, EndKey, WriteOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>BeginKey = binary()</code></li><li><code>EndKey = binary()</code></li><li><code>WriteOpts = <a href="#type-write_options">write_options()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

Removes the database entries in the range ["BeginKey", "EndKey").
like `delete_range/3` but for a column family

<a name="destroy-2"></a>

### destroy/2 ###

<pre><code>
destroy(Name::<a href="http://www.erlang.org/edoc/doc/kernel/doc/file.html#type-filename_all">file:filename_all()</a>, DBOpts::<a href="#type-db_options">db_options()</a>) -&gt; ok | {error, any()}
</code></pre>
<br />

Destroy the contents of the specified database.
Be very careful using this method.

<a name="destroy_column_family-1"></a>

### destroy_column_family/1 ###

`destroy_column_family(CFHandle) -> any()`

<a name="destroy_column_family-2"></a>

### destroy_column_family/2 ###

<pre><code>
destroy_column_family(DBHandle, CFHandle) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

Destroy a column family

<a name="destroy_env-1"></a>

### destroy_env/1 ###

<pre><code>
destroy_env(Env::<a href="#type-env_handle">env_handle()</a>) -&gt; ok
</code></pre>
<br />

destroy an environment

<a name="drop_column_family-1"></a>

### drop_column_family/1 ###

`drop_column_family(CFHandle) -> any()`

<a name="drop_column_family-2"></a>

### drop_column_family/2 ###

<pre><code>
drop_column_family(DBHandle, CFHandle) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

Drop a column family

<a name="flush-2"></a>

### flush/2 ###

<pre><code>
flush(DbHandle::<a href="#type-db_handle">db_handle()</a>, FlushOptions::<a href="#type-flush_options">flush_options()</a>) -&gt; ok | {error, term()}
</code></pre>
<br />

Flush all mem-table data.

<a name="flush-3"></a>

### flush/3 ###

<pre><code>
flush(DbHandle::<a href="#type-db_handle">db_handle()</a>, Cf::<a href="#type-column_family">column_family()</a>, FlushOptions::<a href="#type-flush_options">flush_options()</a>) -&gt; ok | {error, term()}
</code></pre>
<br />

Flush all mem-table data for a column family

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

this function is deprecated and will be removed in next major release.
You should use the `iterator` API instead.

<a name="fold-5"></a>

### fold/5 ###

<pre><code>
fold(DBHandle, CFHandle, Fun, AccIn, ReadOpts) -&gt; AccOut
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>Fun = <a href="#type-fold_fun">fold_fun()</a></code></li><li><code>AccIn = any()</code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>AccOut = any()</code></li></ul>

Calls Fun(Elem, AccIn) on successive elements in the specified column family
Other specs are same with fold/4

this function is deprecated and will be removed in next major release.
You should use the `iterator` API instead.

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

this function is deprecated and will be removed in next major release.
You should use the `iterator` API instead.

<a name="fold_keys-5"></a>

### fold_keys/5 ###

<pre><code>
fold_keys(DBHandle, CFHandle, Fun, AccIn, ReadOpts) -&gt; AccOut
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>Fun = <a href="#type-fold_keys_fun">fold_keys_fun()</a></code></li><li><code>AccIn = any()</code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>AccOut = any()</code></li></ul>

Calls Fun(Elem, AccIn) on successive elements in the specified column family
Other specs are same with fold_keys/4

this function is deprecated and will be removed in next major release.
You should use the `iterator` API instead.

<a name="gc_backup_engine-1"></a>

### gc_backup_engine/1 ###

<pre><code>
gc_backup_engine(BackupEngine::<a href="#type-backup_engine">backup_engine()</a>) -&gt; ok
</code></pre>
<br />

Will delete all the files we don't need anymore
It will do the full scan of the files/ directory and delete all the
files that are not referenced.

<a name="get-3"></a>

### get/3 ###

<pre><code>
get(DBHandle, Key, ReadOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>Key = binary()</code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>Res = {ok, binary()} | not_found | {error, {corruption, string()}} | {error, any()}</code></li></ul>

Retrieve a key/value pair in the default column family

<a name="get-4"></a>

### get/4 ###

<pre><code>
get(DBHandle, CFHandle, Key, ReadOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>Key = binary()</code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>Res = {ok, binary()} | not_found | {error, {corruption, string()}} | {error, any()}</code></li></ul>

Retrieve a key/value pair in the specified column family

<a name="get_approximate_memtable_stats-3"></a>

### get_approximate_memtable_stats/3 ###

<pre><code>
get_approximate_memtable_stats(DBHandle, StartKey, LimitKey) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>StartKey = binary()</code></li><li><code>LimitKey = binary()</code></li><li><code>Res = {ok, {Count::non_neg_integer(), Size::non_neg_integer()}}</code></li></ul>

The method is similar to GetApproximateSizes, except it
returns approximate number of records in memtables.

<a name="get_approximate_memtable_stats-4"></a>

### get_approximate_memtable_stats/4 ###

<pre><code>
get_approximate_memtable_stats(DBHandle, CFHandle, StartKey, LimitKey) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>StartKey = binary()</code></li><li><code>LimitKey = binary()</code></li><li><code>Res = {ok, {Count::non_neg_integer(), Size::non_neg_integer()}}</code></li></ul>

<a name="get_approximate_sizes-3"></a>

### get_approximate_sizes/3 ###

<pre><code>
get_approximate_sizes(DBHandle, Ranges, IncludeFlags) -&gt; Sizes
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>Ranges = [<a href="#type-range">range()</a>]</code></li><li><code>IncludeFlags = <a href="#type-size_approximation_flag">size_approximation_flag()</a></code></li><li><code>Sizes = [non_neg_integer()]</code></li></ul>

For each i in [0,n-1], store in "Sizes[i]", the approximate
file system space used by keys in "[range[i].start .. range[i].limit)".

Note that the returned sizes measure file system space usage, so
if the user data compresses by a factor of ten, the returned
sizes will be one-tenth the size of the corresponding user data size.

If `IncludeFlags` defines whether the returned size should include
the recently written data in the mem-tables (if
the mem-table type supports it), data serialized to disk, or both.

<a name="get_approximate_sizes-4"></a>

### get_approximate_sizes/4 ###

<pre><code>
get_approximate_sizes(DBHandle, CFHandle, Ranges, IncludeFlags) -&gt; Sizes
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>Ranges = [<a href="#type-range">range()</a>]</code></li><li><code>IncludeFlags = <a href="#type-size_approximation_flag">size_approximation_flag()</a></code></li><li><code>Sizes = [non_neg_integer()]</code></li></ul>

<a name="get_backup_info-1"></a>

### get_backup_info/1 ###

<pre><code>
get_backup_info(BackupEngine::<a href="#type-backup_engine">backup_engine()</a>) -&gt; [<a href="#type-backup_info">backup_info()</a>]
</code></pre>
<br />

Returns info about backups in backup_info

<a name="get_capacity-1"></a>

### get_capacity/1 ###

`get_capacity(Cache) -> any()`

<a name="get_latest_sequence_number-1"></a>

### get_latest_sequence_number/1 ###

<pre><code>
get_latest_sequence_number(Db::<a href="#type-db_handle">db_handle()</a>) -&gt; Seq::non_neg_integer()
</code></pre>
<br />

gThe sequence number of the most recent transaction.

<a name="get_pinned_usage-1"></a>

### get_pinned_usage/1 ###

`get_pinned_usage(Cache) -> any()`

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
get_property(DBHandle::<a href="#type-db_handle">db_handle()</a>, CFHandle::<a href="#type-cf_handle">cf_handle()</a>, Property::binary()) -&gt; {ok, binary()} | {error, any()}
</code></pre>
<br />

Return the RocksDB internal status of the specified column family specified at Property

<a name="get_snapshot_sequence-1"></a>

### get_snapshot_sequence/1 ###

<pre><code>
get_snapshot_sequence(SnapshotHandle::<a href="#type-snapshot_handle">snapshot_handle()</a>) -&gt; Sequence::non_neg_integer()
</code></pre>
<br />

returns Snapshot's sequence number

<a name="get_usage-1"></a>

### get_usage/1 ###

`get_usage(Cache) -> any()`

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
iterator(DBHandle, CFHandle, ReadOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>Res = {ok, <a href="#type-itr_handle">itr_handle()</a>} | {error, any()}</code></li></ul>

Return a iterator over the contents of the database.
The result of iterator() is initially invalid (caller must
call iterator_move function on the iterator before using it).

<a name="iterator_close-1"></a>

### iterator_close/1 ###

<pre><code>
iterator_close(ITRHandle) -&gt; ok
</code></pre>

<ul class="definitions"><li><code>ITRHandle = <a href="#type-itr_handle">itr_handle()</a></code></li></ul>

Close a iterator

<a name="iterator_move-2"></a>

### iterator_move/2 ###

<pre><code>
iterator_move(ITRHandle, ITRAction) -&gt; {ok, Key::binary(), Value::binary()} | {ok, Key::binary()} | {error, invalid_iterator} | {error, iterator_closed}
</code></pre>

<ul class="definitions"><li><code>ITRHandle = <a href="#type-itr_handle">itr_handle()</a></code></li><li><code>ITRAction = <a href="#type-iterator_action">iterator_action()</a></code></li></ul>

Move to the specified place

<a name="iterator_refresh-1"></a>

### iterator_refresh/1 ###

<pre><code>
iterator_refresh(ITRHandle) -&gt; ok
</code></pre>

<ul class="definitions"><li><code>ITRHandle = <a href="#type-itr_handle">itr_handle()</a></code></li></ul>

Refresh iterator

<a name="iterators-3"></a>

### iterators/3 ###

<pre><code>
iterators(DBHandle, CFHandle, ReadOpts) -&gt; {ok, <a href="#type-itr_handle">itr_handle()</a>} | {error, any()}
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li></ul>

Return a iterator over the contents of the specified column family.

<a name="list_column_families-2"></a>

### list_column_families/2 ###

<pre><code>
list_column_families(Name, DBOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>Name = <a href="http://www.erlang.org/edoc/doc/kernel/doc/file.html#type-filename_all">file:filename_all()</a></code></li><li><code>DBOpts = <a href="#type-db_options">db_options()</a></code></li><li><code>Res = {ok, [string()]} | {error, any()}</code></li></ul>

List column families

<a name="mem_env-0"></a>

### mem_env/0 ###

`mem_env() -> any()`

<a name="merge-4"></a>

### merge/4 ###

<pre><code>
merge(DBHandle, Key, Value, WriteOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>Key = binary()</code></li><li><code>Value = binary()</code></li><li><code>WriteOpts = <a href="#type-write_options">write_options()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

Merge a key/value pair into the default column family

<a name="merge-5"></a>

### merge/5 ###

<pre><code>
merge(DBHandle, CFHandle, Key, Value, WriteOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>Key = binary()</code></li><li><code>Value = binary()</code></li><li><code>WriteOpts = <a href="#type-write_options">write_options()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

Merge a key/value pair into the specified column family

<a name="new_cache-2"></a>

### new_cache/2 ###

<pre><code>
new_cache(Type::<a href="#type-cache_type">cache_type()</a>, Capacity::non_neg_integer()) -&gt; {ok, <a href="#type-cache_handle">cache_handle()</a>}
</code></pre>
<br />

// Create a new cache.

<a name="new_clock_cache-1"></a>

### new_clock_cache/1 ###

`new_clock_cache(Capacity) -> any()`

<a name="new_env-0"></a>

### new_env/0 ###

<pre><code>
new_env() -&gt; {ok, <a href="#type-env_handle">env_handle()</a>}
</code></pre>
<br />

return a default db environment

<a name="new_env-1"></a>

### new_env/1 ###

<pre><code>
new_env(EnvType::<a href="#type-env_type">env_type()</a>) -&gt; {ok, <a href="#type-env_handle">env_handle()</a>}
</code></pre>
<br />

return a db environment

<a name="new_lru_cache-1"></a>

### new_lru_cache/1 ###

`new_lru_cache(Capacity) -> any()`

<a name="new_rate_limiter-2"></a>

### new_rate_limiter/2 ###

`new_rate_limiter(RateBytesPerSec, Auto) -> any()`

create new Limiter

<a name="new_sst_file_manager-1"></a>

### new_sst_file_manager/1 ###

<pre><code>
new_sst_file_manager(Env::<a href="#type-env_handle">env_handle()</a>) -&gt; {ok, <a href="#type-sst_file_manager">sst_file_manager()</a>} | {error, any()}
</code></pre>
<br />

create new SstFileManager with the default options:
RateBytesPerSec = 0, MaxTrashDbRatio = 0.25, BytesMaxDeleteChunk = 64 * 1024 * 1024.

<a name="new_sst_file_manager-2"></a>

### new_sst_file_manager/2 ###

<pre><code>
new_sst_file_manager(Env, OptionsList) -&gt; Result
</code></pre>

<ul class="definitions"><li><code>Env = <a href="#type-env_handle">env_handle()</a></code></li><li><code>OptionsList = [OptionTuple]</code></li><li><code>OptionTuple = {delete_rate_bytes_per_sec, non_neg_integer()} | {max_trash_db_ratio, float()} | {bytes_max_delete_chunk, non_neg_integer()}</code></li><li><code>Result = {ok, <a href="#type-sst_file_manager">sst_file_manager()</a>} | {error, any()}</code></li></ul>

create new SstFileManager that can be shared among multiple RocksDB
instances to track SST file and control there deletion rate.

* `Env` is an environment resource created using `rocksdb:new_env/{0,1}`.
* `delete_rate_bytes_per_sec`: How many bytes should be deleted per second, If
this value is set to 1024 (1 Kb / sec) and we deleted a file of size 4 Kb
in 1 second, we will wait for another 3 seconds before we delete other
files, Set to 0 to disable deletion rate limiting.
* `max_trash_db_ratio`:  If the trash size constitutes for more than this
fraction of the total DB size we will start deleting new files passed to
DeleteScheduler immediately
* `bytes_max_delete_chunk`:  if a file to delete is larger than delete
chunk, ftruncate the file by this size each time, rather than dropping the
whole file. 0 means to always delete the whole file. If the file has more
than one linked names, the file will be deleted as a whole. Either way,
`delete_rate_bytes_per_sec` will be appreciated. NOTE that with this option,
files already renamed as a trash may be partial, so users should not
directly recover them without checking.

<a name="new_statistics-0"></a>

### new_statistics/0 ###

<pre><code>
new_statistics() -&gt; {ok, <a href="#type-statistics_handle">statistics_handle()</a>}
</code></pre>
<br />

<a name="new_write_buffer_manager-1"></a>

### new_write_buffer_manager/1 ###

<pre><code>
new_write_buffer_manager(BufferSize::non_neg_integer()) -&gt; {ok, <a href="#type-write_buffer_manager">write_buffer_manager()</a>}
</code></pre>
<br />

create a new WriteBufferManager.

<a name="new_write_buffer_manager-2"></a>

### new_write_buffer_manager/2 ###

<pre><code>
new_write_buffer_manager(BufferSize::non_neg_integer(), Cache::<a href="#type-cache_handle">cache_handle()</a>) -&gt; {ok, <a href="#type-write_buffer_manager">write_buffer_manager()</a>}
</code></pre>
<br />

create a new WriteBufferManager. a  WriteBufferManager is for managing memory
allocation for one or more MemTables.

The memory usage of memtable will report to this object. The same object
can be passed into multiple DBs and it will track the sum of size of all
the DBs. If the total size of all live memtables of all the DBs exceeds
a limit, a flush will be triggered in the next DB to which the next write
is issued.

If the object is only passed to on DB, the behavior is the same as
db_write_buffer_size. When write_buffer_manager is set, the value set will
override db_write_buffer_size.

<a name="next_binary_update-1"></a>

### next_binary_update/1 ###

`next_binary_update(Itr) -> any()`

<a name="next_update-1"></a>

### next_update/1 ###

`next_update(Itr) -> any()`

<a name="open-2"></a>

### open/2 ###

<pre><code>
open(Name, DBOpts) -&gt; Result
</code></pre>

<ul class="definitions"><li><code>Name = <a href="http://www.erlang.org/edoc/doc/kernel/doc/file.html#type-filename_all">file:filename_all()</a></code></li><li><code>DBOpts = <a href="#type-options">options()</a></code></li><li><code>Result = {ok, <a href="#type-db_handle">db_handle()</a>} | {error, any()}</code></li></ul>

Open RocksDB with the defalut column family

<a name="open-3"></a>

### open/3 ###

<pre><code>
open(Name, DBOpts, CFDescriptors) -&gt; {ok, <a href="#type-db_handle">db_handle()</a>, [<a href="#type-cf_handle">cf_handle()</a>]} | {error, any()}
</code></pre>

<ul class="definitions"><li><code>Name = <a href="http://www.erlang.org/edoc/doc/kernel/doc/file.html#type-filename_all">file:filename_all()</a></code></li><li><code>DBOpts = <a href="#type-db_options">db_options()</a></code></li><li><code>CFDescriptors = [<a href="#type-cf_descriptor">cf_descriptor()</a>]</code></li></ul>

Open RocksDB with the specified column families

<a name="open_backup_engine-1"></a>

### open_backup_engine/1 ###

<pre><code>
open_backup_engine(Path::string) -&gt; {ok, <a href="#type-backup_engine">backup_engine()</a>} | {error, term()}
</code></pre>
<br />

open a new backup engine for creating new backups.

<a name="open_optimistic_transaction_db-2"></a>

### open_optimistic_transaction_db/2 ###

`open_optimistic_transaction_db(Name, DbOpts) -> any()`

<a name="open_optimistic_transaction_db-3"></a>

### open_optimistic_transaction_db/3 ###

`open_optimistic_transaction_db(Name, DbOpts, CFDescriptors) -> any()`

<a name="open_readonly-2"></a>

### open_readonly/2 ###

<pre><code>
open_readonly(Name, DBOpts) -&gt; Result
</code></pre>

<ul class="definitions"><li><code>Name = <a href="http://www.erlang.org/edoc/doc/kernel/doc/file.html#type-filename_all">file:filename_all()</a></code></li><li><code>DBOpts = <a href="#type-options">options()</a></code></li><li><code>Result = {ok, <a href="#type-db_handle">db_handle()</a>} | {error, any()}</code></li></ul>

<a name="open_readonly-3"></a>

### open_readonly/3 ###

<pre><code>
open_readonly(Name, DBOpts, CFDescriptors) -&gt; {ok, <a href="#type-db_handle">db_handle()</a>, [<a href="#type-cf_handle">cf_handle()</a>]} | {error, any()}
</code></pre>

<ul class="definitions"><li><code>Name = <a href="http://www.erlang.org/edoc/doc/kernel/doc/file.html#type-filename_all">file:filename_all()</a></code></li><li><code>DBOpts = <a href="#type-db_options">db_options()</a></code></li><li><code>CFDescriptors = [<a href="#type-cf_descriptor">cf_descriptor()</a>]</code></li></ul>

Open read-only RocksDB with the specified column families

<a name="open_with_cf-3"></a>

### open_with_cf/3 ###

`open_with_cf(Name, DbOpts, CFDescriptors) -> any()`

<a name="open_with_cf_readonly-3"></a>

### open_with_cf_readonly/3 ###

`open_with_cf_readonly(Name, DbOpts, CFDescriptors) -> any()`

<a name="open_with_ttl-4"></a>

### open_with_ttl/4 ###

<pre><code>
open_with_ttl(Name, DBOpts, TTL, ReadOnly) -&gt; {ok, <a href="#type-db_handle">db_handle()</a>} | {error, any()}
</code></pre>

<ul class="definitions"><li><code>Name = <a href="http://www.erlang.org/edoc/doc/kernel/doc/file.html#type-filename_all">file:filename_all()</a></code></li><li><code>DBOpts = <a href="#type-db_options">db_options()</a></code></li><li><code>TTL = integer()</code></li><li><code>ReadOnly = boolean()</code></li></ul>

Open RocksDB with TTL support
This API should be used to open the db when key-values inserted are
meant to be removed from the db in a non-strict `TTL` amount of time
Therefore, this guarantees that key-values inserted will remain in the
db for >= TTL amount of time and the db will make efforts to remove the
key-values as soon as possible after ttl seconds of their insertion.

BEHAVIOUR:
TTL is accepted in seconds
(int32_t)Timestamp(creation) is suffixed to values in Put internally
Expired TTL values deleted in compaction only:(`Timestamp+ttl<time_now`)
Get/Iterator may return expired entries(compaction not run on them yet)
Different TTL may be used during different Opens
Example: Open1 at t=0 with TTL=4 and insert k1,k2, close at t=2
Open2 at t=3 with TTL=5. Now k1,k2 should be deleted at t>=5
Readonly=true opens in the usual read-only mode. Compactions will not be
triggered(neither manual nor automatic), so no expired entries removed

<a name="purge_old_backup-2"></a>

### purge_old_backup/2 ###

<pre><code>
purge_old_backup(BackupEngine::<a href="#type-backup_engine">backup_engine()</a>, NumBackupToKeep::non_neg_integer()) -&gt; ok | {error, any()}
</code></pre>
<br />

deletes old backups, keeping latest num_backups_to_keep alive

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

<a name="release_batch-1"></a>

### release_batch/1 ###

<pre><code>
release_batch(Batch::<a href="#type-batch_handle">batch_handle()</a>) -&gt; ok
</code></pre>
<br />

<a name="release_cache-1"></a>

### release_cache/1 ###

`release_cache(Cache) -> any()`

release the cache

<a name="release_rate_limiter-1"></a>

### release_rate_limiter/1 ###

`release_rate_limiter(Limiter) -> any()`

release the limiter

<a name="release_snapshot-1"></a>

### release_snapshot/1 ###

<pre><code>
release_snapshot(SnapshotHandle::<a href="#type-snapshot_handle">snapshot_handle()</a>) -&gt; ok | {error, any()}
</code></pre>
<br />

release a snapshot

<a name="release_sst_file_manager-1"></a>

### release_sst_file_manager/1 ###

<pre><code>
release_sst_file_manager(SstFileManager::<a href="#type-sst_file_manager">sst_file_manager()</a>) -&gt; ok
</code></pre>
<br />

release the SstFileManager

<a name="release_statistics-1"></a>

### release_statistics/1 ###

<pre><code>
release_statistics(Statistics::<a href="#type-statistics_handle">statistics_handle()</a>) -&gt; ok
</code></pre>
<br />

release the Statistics Handle

<a name="release_transaction-1"></a>

### release_transaction/1 ###

<pre><code>
release_transaction(TransactionHandle::<a href="#type-transaction_handle">transaction_handle()</a>) -&gt; ok
</code></pre>
<br />

release a transaction

<a name="release_write_buffer_manager-1"></a>

### release_write_buffer_manager/1 ###

<pre><code>
release_write_buffer_manager(WriteBufferManager::<a href="#type-write_buffer_manager">write_buffer_manager()</a>) -&gt; ok
</code></pre>
<br />

<a name="repair-2"></a>

### repair/2 ###

<pre><code>
repair(Name::<a href="http://www.erlang.org/edoc/doc/kernel/doc/file.html#type-filename_all">file:filename_all()</a>, DBOpts::<a href="#type-db_options">db_options()</a>) -&gt; ok | {error, any()}
</code></pre>
<br />

Try to repair as much of the contents of the database as possible.
Some data may be lost, so be careful when calling this function

<a name="restore_db_from_backup-3"></a>

### restore_db_from_backup/3 ###

<pre><code>
restore_db_from_backup(BackupEngine, BackupId, DbDir) -&gt; Result
</code></pre>

<ul class="definitions"><li><code>BackupEngine = <a href="#type-backup_engine">backup_engine()</a></code></li><li><code>BackupId = non_neg_integer()</code></li><li><code>DbDir = string()</code></li><li><code>Result = ok | {error, any()}</code></li></ul>

restore from backup with backup_id

<a name="restore_db_from_backup-4"></a>

### restore_db_from_backup/4 ###

<pre><code>
restore_db_from_backup(BackupEngine, BackupId, DbDir, WalDir) -&gt; Result
</code></pre>

<ul class="definitions"><li><code>BackupEngine = <a href="#type-backup_engine">backup_engine()</a></code></li><li><code>BackupId = non_neg_integer()</code></li><li><code>DbDir = string()</code></li><li><code>WalDir = string()</code></li><li><code>Result = ok | {error, any()}</code></li></ul>

restore from backup with backup_id

<a name="restore_db_from_latest_backup-2"></a>

### restore_db_from_latest_backup/2 ###

<pre><code>
restore_db_from_latest_backup(BackupEngine, DbDir) -&gt; Result
</code></pre>

<ul class="definitions"><li><code>BackupEngine = <a href="#type-backup_engine">backup_engine()</a></code></li><li><code>DbDir = string()</code></li><li><code>Result = ok | {error, any()}</code></li></ul>

restore from the latest backup

<a name="restore_db_from_latest_backup-3"></a>

### restore_db_from_latest_backup/3 ###

<pre><code>
restore_db_from_latest_backup(BackupEngine, DbDir, WalDir) -&gt; Result
</code></pre>

<ul class="definitions"><li><code>BackupEngine = <a href="#type-backup_engine">backup_engine()</a></code></li><li><code>DbDir = string()</code></li><li><code>WalDir = string()</code></li><li><code>Result = ok | {error, any()}</code></li></ul>

restore from the latest backup

<a name="set_capacity-2"></a>

### set_capacity/2 ###

<pre><code>
set_capacity(Cache::<a href="#type-cache_handle">cache_handle()</a>, Capacity::non_neg_integer()) -&gt; ok
</code></pre>
<br />

sets the maximum configured capacity of the cache. When the new
capacity is less than the old capacity and the existing usage is
greater than new capacity, the implementation will do its best job to
purge the released entries from the cache in order to lower the usage

<a name="set_db_background_threads-2"></a>

### set_db_background_threads/2 ###

<pre><code>
set_db_background_threads(DB::<a href="#type-db_handle">db_handle()</a>, N::non_neg_integer()) -&gt; ok
</code></pre>
<br />

set background threads of a database

<a name="set_db_background_threads-3"></a>

### set_db_background_threads/3 ###

<pre><code>
set_db_background_threads(DB::<a href="#type-db_handle">db_handle()</a>, N::non_neg_integer(), Priority::<a href="#type-env_priority">env_priority()</a>) -&gt; ok
</code></pre>
<br />

set database background threads of low and high prioriry threads pool of an environment
Flush threads are in the HIGH priority pool, while compaction threads are in the
LOW priority pool. To increase the number of threads in each pool call:

<a name="set_env_background_threads-2"></a>

### set_env_background_threads/2 ###

<pre><code>
set_env_background_threads(Env::<a href="#type-env_handle">env_handle()</a>, N::non_neg_integer()) -&gt; ok
</code></pre>
<br />

set background threads of an environment

<a name="set_env_background_threads-3"></a>

### set_env_background_threads/3 ###

<pre><code>
set_env_background_threads(Env::<a href="#type-env_handle">env_handle()</a>, N::non_neg_integer(), Priority::<a href="#type-env_priority">env_priority()</a>) -&gt; ok
</code></pre>
<br />

set background threads of low and high prioriry threads pool of an environment
Flush threads are in the HIGH priority pool, while compaction threads are in the
LOW priority pool. To increase the number of threads in each pool call:

<a name="set_stats_level-2"></a>

### set_stats_level/2 ###

<pre><code>
set_stats_level(StatisticsHandle::<a href="#type-statistics_handle">statistics_handle()</a>, StatsLevel::<a href="#type-stats_level">stats_level()</a>) -&gt; ok
</code></pre>
<br />

<a name="set_strict_capacity_limit-2"></a>

### set_strict_capacity_limit/2 ###

<pre><code>
set_strict_capacity_limit(Cache::<a href="#type-cache_handle">cache_handle()</a>, StrictCapacityLimit::boolean()) -&gt; ok
</code></pre>
<br />

sets strict_capacity_limit flag of the cache. If the flag is set
to true, insert to cache will fail if no enough capacity can be free.

<a name="single_delete-3"></a>

### single_delete/3 ###

<pre><code>
single_delete(DBHandle, Key, WriteOpts) -&gt; ok | {error, any()}
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>Key = binary()</code></li><li><code>WriteOpts = <a href="#type-write_options">write_options()</a></code></li></ul>

Remove the database entry for "key". Requires that the key exists
and was not overwritten. Returns OK on success, and a non-OK status
on error.  It is not an error if "key" did not exist in the database.

If a key is overwritten (by calling Put() multiple times), then the result
of calling SingleDelete() on this key is undefined.  SingleDelete() only
behaves correctly if there has been only one Put() for this key since the
previous call to SingleDelete() for this key.

This feature is currently an experimental performance optimization
for a very specific workload.  It is up to the caller to ensure that
SingleDelete is only used for a key that is not deleted using Delete() or
written using Merge().  Mixing SingleDelete operations with Deletes
can result in undefined behavior.

Note: consider setting options `{sync, true}`.

<a name="single_delete-4"></a>

### single_delete/4 ###

<pre><code>
single_delete(DBHandle, CFHandle, Key, WriteOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>Key = binary()</code></li><li><code>WriteOpts = <a href="#type-write_options">write_options()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

like `single_delete/3` but on the specified column family

<a name="snapshot-1"></a>

### snapshot/1 ###

<pre><code>
snapshot(DbHandle::<a href="#type-db_handle">db_handle()</a>) -&gt; {ok, <a href="#type-snapshot_handle">snapshot_handle()</a>} | {error, any()}
</code></pre>
<br />

return a database snapshot
Snapshots provide consistent read-only views over the entire state of the key-value store

<a name="sst_file_manager_flag-3"></a>

### sst_file_manager_flag/3 ###

<pre><code>
sst_file_manager_flag(SstFileManager, Flag, Value) -&gt; Result
</code></pre>

<ul class="definitions"><li><code>SstFileManager = <a href="#type-sst_file_manager">sst_file_manager()</a></code></li><li><code>Flag = max_allowed_space_usage | compaction_buffer_size | delete_rate_bytes_per_sec | max_trash_db_ratio</code></li><li><code>Value = non_neg_integer() | float()</code></li><li><code>Result = ok</code></li></ul>

set certains flags for the SST file manager
* `max_allowed_space_usage`: Update the maximum allowed space that should be used by RocksDB, if
the total size of the SST files exceeds MaxAllowedSpace, writes to
RocksDB will fail.

Setting MaxAllowedSpace to 0 will disable this feature; maximum allowed
pace will be infinite (Default value).
* `compaction_buffer_size`: Set the amount of buffer room each compaction should be able to leave.
In other words, at its maximum disk space consumption, the compaction
should still leave compaction_buffer_size available on the disk so that
other background functions may continue, such as logging and flushing.
* `delete_rate_bytes_per_sec`: Update the delete rate limit in bytes per second.
zero means disable delete rate limiting and delete files immediately
* `max_trash_db_ratio`: Update trash/DB size ratio where new files will be deleted immediately (float)

<a name="sst_file_manager_info-1"></a>

### sst_file_manager_info/1 ###

<pre><code>
sst_file_manager_info(SstFileManager) -&gt; InfoList
</code></pre>

<ul class="definitions"><li><code>SstFileManager = <a href="#type-sst_file_manager">sst_file_manager()</a></code></li><li><code>InfoList = [InfoTuple]</code></li><li><code>InfoTuple = {total_size, non_neg_integer()} | {delete_rate_bytes_per_sec, non_neg_integer()} | {max_trash_db_ratio, float()} | {total_trash_size, non_neg_integer()} | {is_max_allowed_space_reached, boolean()} | {max_allowed_space_reached_including_compactions, boolean()}</code></li></ul>

return informations of a Sst File Manager as a list of tuples.

* `{total_size, Int>0}`: total size of all tracked files
* `{delete_rate_bytes_per_sec, Int > 0}`: delete rate limit in bytes per second
* `{max_trash_db_ratio, Float>0}`: trash/DB size ratio where new files will be deleted immediately
* `{total_trash_size, Int > 0}`: total size of trash files
* `{is_max_allowed_space_reached, Boolean}` true if the total size of SST files exceeded the maximum allowed space usage
* `{max_allowed_space_reached_including_compactions, Boolean}`: true if the total size of SST files as well as
estimated size of ongoing compactions exceeds the maximums allowed space usage

<a name="sst_file_manager_info-2"></a>

### sst_file_manager_info/2 ###

<pre><code>
sst_file_manager_info(SstFileManager, Item) -&gt; Value
</code></pre>

<ul class="definitions"><li><code>SstFileManager = <a href="#type-sst_file_manager">sst_file_manager()</a></code></li><li><code>Item = total_size | delete_rate_bytes_per_sec | max_trash_db_ratio | total_trash_size | is_max_allowed_space_reached | max_allowed_space_reached_including_compactions</code></li><li><code>Value = term()</code></li></ul>

return the information associated with Item for an SST File Manager SstFileManager

<a name="statistics_info-1"></a>

### statistics_info/1 ###

<pre><code>
statistics_info(Statistics) -&gt; InfoList
</code></pre>

<ul class="definitions"><li><code>Statistics = <a href="#type-statistics_handle">statistics_handle()</a></code></li><li><code>InfoList = [InfoTuple]</code></li><li><code>InfoTuple = {stats_level, <a href="#type-stats_level">stats_level()</a>}</code></li></ul>

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

<a name="stop_backup-1"></a>

### stop_backup/1 ###

<pre><code>
stop_backup(BackupEngine::<a href="#type-backup_engine">backup_engine()</a>) -&gt; ok
</code></pre>
<br />

<a name="sync_wal-1"></a>

### sync_wal/1 ###

<pre><code>
sync_wal(DbHandle::<a href="#type-db_handle">db_handle()</a>) -&gt; ok | {error, term()}
</code></pre>
<br />

Sync the wal. Note that Write() followed by SyncWAL() is not exactly the
same as Write() with sync=true: in the latter case the changes won't be
visible until the sync is done.
Currently only works if allow_mmap_writes = false in Options.

<a name="tlog_iterator-2"></a>

### tlog_iterator/2 ###

<pre><code>
tlog_iterator(Db::<a href="#type-db_handle">db_handle()</a>, Since::non_neg_integer()) -&gt; {ok, Iterator::term()}
</code></pre>
<br />

create a new iterator to retrive ethe transaction log since a sequce

<a name="tlog_iterator_close-1"></a>

### tlog_iterator_close/1 ###

<pre><code>
tlog_iterator_close(Iterator::term()) -&gt; ok
</code></pre>
<br />

close the transaction log

<a name="tlog_next_binary_update-1"></a>

### tlog_next_binary_update/1 ###

<pre><code>
tlog_next_binary_update(Iterator::term()) -&gt; {ok, LastSeq::non_neg_integer(), BinLog::binary()} | {error, term()}
</code></pre>
<br />

go to the last update as a binary in the transaction log, can be ussed with the write_binary_update function.

<a name="tlog_next_update-1"></a>

### tlog_next_update/1 ###

<pre><code>
tlog_next_update(Iterator::term()) -&gt; {ok, LastSeq::non_neg_integer(), Log::<a href="#type-write_actions">write_actions()</a>, BinLog::binary()} | {error, term()}
</code></pre>
<br />

like `tlog_nex_binary_update/1` but also return the batch as a list of operations

<a name="transaction-2"></a>

### transaction/2 ###

<pre><code>
transaction(TransactionDB::<a href="#type-db_handle">db_handle()</a>, WriteOptions::<a href="#type-write_options">write_options()</a>) -&gt; {ok, <a href="#type-transaction_handle">transaction_handle()</a>}
</code></pre>
<br />

create a new transaction
When opened as a Transaction or Optimistic Transaction db,
a user can both read and write to a transaction without committing
anything to the disk until they decide to do so.

<a name="transaction_commit-1"></a>

### transaction_commit/1 ###

<pre><code>
transaction_commit(Transaction::<a href="#type-transaction_handle">transaction_handle()</a>) -&gt; ok | {error, term()}
</code></pre>
<br />

commit a transaction to disk atomically (?)

<a name="transaction_delete-2"></a>

### transaction_delete/2 ###

<pre><code>
transaction_delete(Transaction::<a href="#type-transaction_handle">transaction_handle()</a>, Key::binary()) -&gt; ok
</code></pre>
<br />

transaction implementation of delete operation to the transaction

<a name="transaction_delete-3"></a>

### transaction_delete/3 ###

<pre><code>
transaction_delete(Transaction::<a href="#type-transaction_handle">transaction_handle()</a>, ColumnFamily::<a href="#type-cf_handle">cf_handle()</a>, Key::binary()) -&gt; ok
</code></pre>
<br />

like `transaction_delete/2` but apply the operation to a column family

<a name="transaction_get-3"></a>

### transaction_get/3 ###

<pre><code>
transaction_get(Transaction::<a href="#type-transaction_handle">transaction_handle()</a>, Key::binary(), Opts::<a href="#type-read_options">read_options()</a>) -&gt; Res::{ok, binary()} | not_found | {error, {corruption, string()}} | {error, any()}
</code></pre>
<br />

do a get operation on the contents of the transaction

<a name="transaction_get-4"></a>

### transaction_get/4 ###

<pre><code>
transaction_get(Transaction::<a href="#type-transaction_handle">transaction_handle()</a>, ColumnFamily::<a href="#type-cf_handle">cf_handle()</a>, Key::binary(), Opts::<a href="#type-read_options">read_options()</a>) -&gt; Res::{ok, binary()} | not_found | {error, {corruption, string()}} | {error, any()}
</code></pre>
<br />

like `transaction_get/3` but apply the operation to a column family

<a name="transaction_iterator-2"></a>

### transaction_iterator/2 ###

<pre><code>
transaction_iterator(TransactionHandle, ReadOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>TransactionHandle = <a href="#type-transaction_handle">transaction_handle()</a></code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>Res = {ok, <a href="#type-itr_handle">itr_handle()</a>} | {error, any()}</code></li></ul>

Return a iterator over the contents of the database and
uncommited writes and deletes in the current transaction.
The result of iterator() is initially invalid (caller must
call iterator_move function on the iterator before using it).

<a name="transaction_iterator-3"></a>

### transaction_iterator/3 ###

<pre><code>
transaction_iterator(TransactionHandle, CFHandle, ReadOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>TransactionHandle = <a href="#type-transaction_handle">transaction_handle()</a></code></li><li><code>CFHandle = <a href="#type-cf_handle">cf_handle()</a></code></li><li><code>ReadOpts = <a href="#type-read_options">read_options()</a></code></li><li><code>Res = {ok, <a href="#type-itr_handle">itr_handle()</a>} | {error, any()}</code></li></ul>

Return a iterator over the contents of the database and
uncommited writes and deletes in the current transaction.
The result of iterator() is initially invalid (caller must
call iterator_move function on the iterator before using it).

<a name="transaction_put-3"></a>

### transaction_put/3 ###

<pre><code>
transaction_put(Transaction::<a href="#type-transaction_handle">transaction_handle()</a>, Key::binary(), Value::binary()) -&gt; ok | {error, any()}
</code></pre>
<br />

add a put operation to the transaction

<a name="transaction_put-4"></a>

### transaction_put/4 ###

<pre><code>
transaction_put(Transaction::<a href="#type-transaction_handle">transaction_handle()</a>, ColumnFamily::<a href="#type-cf_handle">cf_handle()</a>, Key::binary(), Value::binary()) -&gt; ok | {error, any()}
</code></pre>
<br />

like `transaction_put/3` but apply the operation to a column family

<a name="transaction_rollback-1"></a>

### transaction_rollback/1 ###

<pre><code>
transaction_rollback(Transaction::<a href="#type-transaction_handle">transaction_handle()</a>) -&gt; ok | {error, term()}
</code></pre>
<br />

rollback a transaction to disk atomically (?)

<a name="updates_iterator-2"></a>

### updates_iterator/2 ###

`updates_iterator(DBH, Since) -> any()`

<a name="verify_backup-2"></a>

### verify_backup/2 ###

<pre><code>
verify_backup(BackupEngine::<a href="#type-backup_engine">backup_engine()</a>, BackupId::non_neg_integer()) -&gt; ok | {error, any()}
</code></pre>
<br />

checks that each file exists and that the size of the file matches
our expectations. it does not check file checksum.

<a name="write-3"></a>

### write/3 ###

<pre><code>
write(DBHandle, WriteActions, WriteOpts) -&gt; Res
</code></pre>

<ul class="definitions"><li><code>DBHandle = <a href="#type-db_handle">db_handle()</a></code></li><li><code>WriteActions = <a href="#type-write_actions">write_actions()</a></code></li><li><code>WriteOpts = <a href="#type-write_options">write_options()</a></code></li><li><code>Res = ok | {error, any()}</code></li></ul>

Apply the specified updates to the database.
this function will be removed on the next major release. You should use the `batch_*` API instead.

<a name="write_batch-3"></a>

### write_batch/3 ###

<pre><code>
write_batch(Db::<a href="#type-db_handle">db_handle()</a>, Batch::<a href="#type-batch_handle">batch_handle()</a>, WriteOptions::<a href="#type-write_options">write_options()</a>) -&gt; ok | {error, term()}
</code></pre>
<br />

write the batch to the database

<a name="write_binary_update-3"></a>

### write_binary_update/3 ###

<pre><code>
write_binary_update(DbHandle::<a href="#type-db_handle">db_handle()</a>, BinLog::binary(), WriteOptions::<a href="#type-write_options">write_options()</a>) -&gt; ok | {error, term()}
</code></pre>
<br />

apply a set of operation coming from a transaction log to another database. Can be useful to use it in slave
mode.

<a name="write_buffer_manager_info-1"></a>

### write_buffer_manager_info/1 ###

<pre><code>
write_buffer_manager_info(WriteBufferManager) -&gt; InfoList
</code></pre>

<ul class="definitions"><li><code>WriteBufferManager = <a href="#type-write_buffer_manager">write_buffer_manager()</a></code></li><li><code>InfoList = [InfoTuple]</code></li><li><code>InfoTuple = {memory_usage, non_neg_integer()} | {mutable_memtable_memory_usage, non_neg_integer()} | {buffer_size, non_neg_integer()} | {enabled, boolean()}</code></li></ul>

return informations of a Write Buffer Manager as a list of tuples.

<a name="write_buffer_manager_info-2"></a>

### write_buffer_manager_info/2 ###

<pre><code>
write_buffer_manager_info(WriteBufferManager, Item) -&gt; Value
</code></pre>

<ul class="definitions"><li><code>WriteBufferManager = <a href="#type-write_buffer_manager">write_buffer_manager()</a></code></li><li><code>Item = memory_usage | mutable_memtable_memory_usage | buffer_size | enabled</code></li><li><code>Value = term()</code></li></ul>

return the information associated with Item for a Write Buffer Manager.

