

# Module rocksdb_sst_file_manager #
* [Function Index](#index)
* [Function Details](#functions)

<a name="index"></a>

## Function Index ##


<table width="100%" border="1" cellspacing="0" cellpadding="2" summary="function index"><tr><td valign="top"><a href="#get_delete_rate_bytes_per_second-1">get_delete_rate_bytes_per_second/1</a></td><td>Return delete rate limit in bytes per second.</td></tr><tr><td valign="top"><a href="#get_max_trash_db_ratio-1">get_max_trash_db_ratio/1</a></td><td>Return trash/DB size ratio where new files will be deleted immediately.</td></tr><tr><td valign="top"><a href="#get_total_size-1">get_total_size/1</a></td><td>Return the total size of all tracked files.</td></tr><tr><td valign="top"><a href="#get_total_trash_size-1">get_total_trash_size/1</a></td><td>Return the total size of trash files.</td></tr><tr><td valign="top"><a href="#is_max_allowed_space_reached-1">is_max_allowed_space_reached/1</a></td><td>Return true if the total size of SST files exceeded the maximum allowed
space usage.</td></tr><tr><td valign="top"><a href="#is_max_allowed_space_reached_including_compactions-1">is_max_allowed_space_reached_including_compactions/1</a></td><td>Returns true if the total size of SST files as well as estimated size
of ongoing compactions exceeds the maximums allowed space usage.</td></tr><tr><td valign="top"><a href="#set_compaction_buffer_size-2">set_compaction_buffer_size/2</a></td><td>Set the amount of buffer room each compaction should be able to leave.</td></tr><tr><td valign="top"><a href="#set_delete_rate_bytes_per_second-2">set_delete_rate_bytes_per_second/2</a></td><td>Update the delete rate limit in bytes per second.</td></tr><tr><td valign="top"><a href="#set_max_allowed_usage-2">set_max_allowed_usage/2</a></td><td>Update the maximum allowed space that should be used by RocksDB, if
the total size of the SST files exceeds MaxAllowedSpace, writes to
RocksDB will fail.</td></tr><tr><td valign="top"><a href="#set_max_trash_db_ratio-2">set_max_trash_db_ratio/2</a></td><td>Update trash/DB size ratio where new files will be deleted immediately.</td></tr></table>


<a name="functions"></a>

## Function Details ##

<a name="get_delete_rate_bytes_per_second-1"></a>

### get_delete_rate_bytes_per_second/1 ###

<pre><code>
get_delete_rate_bytes_per_second(SstFileManager::<a href="rocksdb.md#type-sst_file_manager">rocksdb:sst_file_manager()</a>) -&gt; non_neg_integer()
</code></pre>
<br />

Return delete rate limit in bytes per second.

<a name="get_max_trash_db_ratio-1"></a>

### get_max_trash_db_ratio/1 ###

<pre><code>
get_max_trash_db_ratio(SstFileManager::<a href="rocksdb.md#type-sst_file_manager">rocksdb:sst_file_manager()</a>) -&gt; float()
</code></pre>
<br />

Return trash/DB size ratio where new files will be deleted immediately

<a name="get_total_size-1"></a>

### get_total_size/1 ###

<pre><code>
get_total_size(SstFileManager::<a href="rocksdb.md#type-sst_file_manager">rocksdb:sst_file_manager()</a>) -&gt; non_neg_integer()
</code></pre>
<br />

Return the total size of all tracked files.

<a name="get_total_trash_size-1"></a>

### get_total_trash_size/1 ###

<pre><code>
get_total_trash_size(SstFileManager::<a href="rocksdb.md#type-sst_file_manager">rocksdb:sst_file_manager()</a>) -&gt; non_neg_integer()
</code></pre>
<br />

Return the total size of trash files

<a name="is_max_allowed_space_reached-1"></a>

### is_max_allowed_space_reached/1 ###

<pre><code>
is_max_allowed_space_reached(SstFileManager::<a href="rocksdb.md#type-sst_file_manager">rocksdb:sst_file_manager()</a>) -&gt; boolean()
</code></pre>
<br />

Return true if the total size of SST files exceeded the maximum allowed
space usage.

<a name="is_max_allowed_space_reached_including_compactions-1"></a>

### is_max_allowed_space_reached_including_compactions/1 ###

<pre><code>
is_max_allowed_space_reached_including_compactions(SstFileManager::<a href="rocksdb.md#type-sst_file_manager">rocksdb:sst_file_manager()</a>) -&gt; boolean()
</code></pre>
<br />

Returns true if the total size of SST files as well as estimated size
of ongoing compactions exceeds the maximums allowed space usage.

<a name="set_compaction_buffer_size-2"></a>

### set_compaction_buffer_size/2 ###

<pre><code>
set_compaction_buffer_size(SstFileManager::<a href="rocksdb.md#type-sst_file_manager">rocksdb:sst_file_manager()</a>, CompactionBufferSize::non_neg_integer()) -&gt; ok
</code></pre>
<br />

Set the amount of buffer room each compaction should be able to leave.
In other words, at its maximum disk space consumption, the compaction
should still leave compaction_buffer_size available on the disk so that
other background functions may continue, such as logging and flushing.

<a name="set_delete_rate_bytes_per_second-2"></a>

### set_delete_rate_bytes_per_second/2 ###

<pre><code>
set_delete_rate_bytes_per_second(SstFileManager::<a href="rocksdb.md#type-sst_file_manager">rocksdb:sst_file_manager()</a>, DeleteRate::non_neg_integer()) -&gt; ok
</code></pre>
<br />

Update the delete rate limit in bytes per second.
zero means disable delete rate limiting and delete files immediately

<a name="set_max_allowed_usage-2"></a>

### set_max_allowed_usage/2 ###

<pre><code>
set_max_allowed_usage(SstFileManager::<a href="rocksdb.md#type-sst_file_manager">rocksdb:sst_file_manager()</a>, MaxAllowedSpace::non_neg_integer()) -&gt; ok
</code></pre>
<br />

Update the maximum allowed space that should be used by RocksDB, if
the total size of the SST files exceeds MaxAllowedSpace, writes to
RocksDB will fail.

setting MaxAllowedSpace to 0 will disable this feature; maximum allowed
pace will be infinite (Default value).

<a name="set_max_trash_db_ratio-2"></a>

### set_max_trash_db_ratio/2 ###

<pre><code>
set_max_trash_db_ratio(SstFileManager::<a href="rocksdb.md#type-sst_file_manager">rocksdb:sst_file_manager()</a>, Ratio::float()) -&gt; ok
</code></pre>
<br />

Update trash/DB size ratio where new files will be deleted immediately

