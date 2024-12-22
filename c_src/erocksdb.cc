// Copyright (c) 2011-2013 Basho Technologies, Inc. All Rights Reserved.
// Copyright (c) 2016-2022 Benoit Chesneau
//
// This file is provided to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file
// except in compliance with the License.  You may obtain
// a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "atoms.h"
#include "erocksdb.h"
#include "refobjects.h"
#include "cache.h"
#include "statistics.h"
#include "rate_limiter.h"
#include "env.h"
#include "sst_file_manager.h"
#include "write_buffer_manager.h"

// See erl_nif(3) Data Types sections for ErlNifFunc for more deails
#define ERL_NIF_REGULAR_BOUND 0

static ErlNifFunc nif_funcs[] =
    {

        {"open", 2, erocksdb::Open, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"open_readonly", 2, erocksdb::OpenReadOnly, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"open", 3, erocksdb::OpenWithCf, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"open_readonly", 3, erocksdb::OpenWithCfReadOnly, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"open_with_ttl", 4, erocksdb::OpenWithTTL, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"open_optimistic_transaction_db", 3,
         erocksdb::OpenOptimisticTransactionDB, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"close", 1, erocksdb::Close, ERL_NIF_DIRTY_JOB_IO_BOUND},

        // db management
        {"checkpoint", 2, erocksdb::Checkpoint, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"repair", 2, erocksdb::Repair, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"is_empty", 1, erocksdb::IsEmpty, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"destroy", 2, erocksdb::Destroy, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"get_property", 2, erocksdb::GetProperty, ERL_NIF_REGULAR_BOUND},
        {"get_property", 3, erocksdb::GetProperty, ERL_NIF_REGULAR_BOUND},
        {"flush", 3, erocksdb::Flush, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"sync_wal", 1, erocksdb::SyncWal, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"set_db_background_threads", 2, erocksdb::SetDBBackgroundThreads, ERL_NIF_REGULAR_BOUND},

        {"get_approximate_sizes", 3, erocksdb::GetApproximateSizes, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"get_approximate_sizes", 4, erocksdb::GetApproximateSizes, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"get_approximate_memtable_stats", 3, erocksdb::GetApproximateMemTableStats, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"get_approximate_memtable_stats", 4, erocksdb::GetApproximateMemTableStats, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"delete_range", 4, erocksdb::DeleteRange, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"delete_range", 5, erocksdb::DeleteRange, ERL_NIF_DIRTY_JOB_IO_BOUND},

        {"compact_range", 4, erocksdb::CompactRange, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"compact_range", 5, erocksdb::CompactRange, ERL_NIF_DIRTY_JOB_IO_BOUND},

        // column families
        {"list_column_families", 2, erocksdb::ListColumnFamilies, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"create_column_family", 3, erocksdb::CreateColumnFamily, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"drop_column_family", 1, erocksdb::DropColumnFamily, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"drop_column_family", 2, erocksdb::DropColumnFamily, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"destroy_column_family", 1, erocksdb::DestroyColumnFamily, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"destroy_column_family", 2, erocksdb::DestroyColumnFamily, ERL_NIF_DIRTY_JOB_IO_BOUND},

        // kv operations
        {"get", 3, erocksdb::Get, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"get", 4, erocksdb::Get, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"put", 4, erocksdb::Put, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"put", 5, erocksdb::Put, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"merge", 4, erocksdb::Merge, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"merge", 5, erocksdb::Merge, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"delete", 3, erocksdb::Delete, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"delete", 4, erocksdb::Delete, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"single_delete", 3, erocksdb::SingleDelete, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"single_delete", 4, erocksdb::SingleDelete, ERL_NIF_DIRTY_JOB_IO_BOUND},

        {"snapshot", 1, erocksdb::Snapshot, ERL_NIF_REGULAR_BOUND},
        {"release_snapshot", 1, erocksdb::ReleaseSnapshot, ERL_NIF_REGULAR_BOUND},
        {"get_snapshot_sequence", 1, erocksdb::GetSnapshotSequenceNumber, ERL_NIF_REGULAR_BOUND},

        // iterator operations
        {"iterator", 2, erocksdb::Iterator, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"iterator", 3, erocksdb::Iterator, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"iterators", 3, erocksdb::Iterators, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"iterator_move", 2, erocksdb::IteratorMove, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"iterator_refresh", 1, erocksdb::IteratorRefresh, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"iterator_close", 1, erocksdb::IteratorClose, ERL_NIF_DIRTY_JOB_IO_BOUND},

        {"get_latest_sequence_number", 1, erocksdb::GetLatestSequenceNumber, ERL_NIF_REGULAR_BOUND},

        // transactions
        {"tlog_iterator", 2, erocksdb::TransactionLogIterator, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"tlog_iterator_close", 1, erocksdb::TransactionLogIteratorClose, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"tlog_next_binary_update", 1, erocksdb::TransactionLogNextBinaryUpdate, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"tlog_next_update", 1, erocksdb::TransactionLogNextUpdate, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"write_binary_update", 3, erocksdb::WriteBinaryUpdate, ERL_NIF_DIRTY_JOB_IO_BOUND},

        // optimistic transaction db

        {"transaction", 2, erocksdb::NewTransaction, ERL_NIF_REGULAR_BOUND},
        {"transaction_put", 3, erocksdb::PutTransaction, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"transaction_put", 4, erocksdb::PutTransaction, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"transaction_get", 3, erocksdb::GetTransaction, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"transaction_get", 4, erocksdb::GetTransaction, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"transaction_delete", 2, erocksdb::DelTransaction, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"transaction_delete", 3, erocksdb::DelTransaction, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"transaction_iterator", 2, erocksdb::IteratorTransaction, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"transaction_iterator", 3, erocksdb::IteratorTransaction, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"transaction_commit", 1, erocksdb::CommitTransaction, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"transaction_rollback", 1, erocksdb::RollbackTransaction, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"release_transaction", 1, erocksdb::ReleaseTransaction, ERL_NIF_REGULAR_BOUND},

        // Batch
        {"batch", 0, erocksdb::NewBatch, ERL_NIF_REGULAR_BOUND},
        {"release_batch", 1, erocksdb::ReleaseBatch, ERL_NIF_REGULAR_BOUND},
        {"write_batch", 3, erocksdb::WriteBatch, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"batch_put", 3, erocksdb::PutBatch, ERL_NIF_REGULAR_BOUND},
        {"batch_put", 4, erocksdb::PutBatch, ERL_NIF_REGULAR_BOUND},
        {"batch_merge", 3, erocksdb::MergeBatch, ERL_NIF_REGULAR_BOUND},
        {"batch_merge", 4, erocksdb::MergeBatch, ERL_NIF_REGULAR_BOUND},
        {"batch_delete", 2, erocksdb::DeleteBatch, ERL_NIF_REGULAR_BOUND},
        {"batch_delete", 3, erocksdb::DeleteBatch, ERL_NIF_REGULAR_BOUND},
        {"batch_single_delete", 2, erocksdb::SingleDeleteBatch, ERL_NIF_REGULAR_BOUND},
        {"batch_single_delete", 3, erocksdb::SingleDeleteBatch, ERL_NIF_REGULAR_BOUND},
        {"batch_clear", 1, erocksdb::ClearBatch, ERL_NIF_REGULAR_BOUND},
        {"batch_savepoint", 1, erocksdb::BatchSetSavePoint, ERL_NIF_REGULAR_BOUND},
        {"batch_rollback", 1, erocksdb::BatchRollbackToSavePoint, ERL_NIF_REGULAR_BOUND},
        {"batch_count", 1, erocksdb::BatchCount, ERL_NIF_REGULAR_BOUND},
        {"batch_data_size", 1, erocksdb::BatchDataSize, ERL_NIF_REGULAR_BOUND},
        {"batch_tolist", 1, erocksdb::BatchToList, ERL_NIF_DIRTY_JOB_CPU_BOUND},

        // backup engine
        {"open_backup_engine", 1, erocksdb::OpenBackupEngine, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"create_new_backup", 2, erocksdb::CreateNewBackup, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"get_backup_info", 1, erocksdb::GetBackupInfo, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"verify_backup", 2, erocksdb::VerifyBackup, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"stop_backup", 1, erocksdb::StopBackup, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"delete_backup", 2, erocksdb::DeleteBackup, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"purge_old_backup", 2, erocksdb::PurgeOldBackup, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"restore_db_from_backup", 3, erocksdb::RestoreDBFromBackup, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"restore_db_from_backup", 4, erocksdb::RestoreDBFromBackup, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"restore_db_from_latest_backup", 2, erocksdb::RestoreDBFromLatestBackup, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"restore_db_from_latest_backup", 3, erocksdb::RestoreDBFromLatestBackup, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"gc_backup_engine", 1, erocksdb::GCBackupEngine, ERL_NIF_DIRTY_JOB_IO_BOUND},
        {"close_backup_engine", 1, erocksdb::CloseBackupEngine, ERL_NIF_DIRTY_JOB_IO_BOUND},

        // cache
        {"new_cache", 2, erocksdb::NewCache, ERL_NIF_REGULAR_BOUND},
        {"cache_info", 1, erocksdb::CacheInfo, ERL_NIF_REGULAR_BOUND},
        {"cache_info", 2, erocksdb::CacheInfo, ERL_NIF_REGULAR_BOUND},
        {"release_cache", 1, erocksdb::ReleaseCache, ERL_NIF_REGULAR_BOUND},
        {"set_strict_capacity_limit", 2, erocksdb::SetStrictCapacityLimit, ERL_NIF_REGULAR_BOUND},
        {"set_capacity", 2, erocksdb::SetCapacity, ERL_NIF_DIRTY_JOB_CPU_BOUND},

        // rate limiter
        {"new_rate_limiter", 2, erocksdb::NewRateLimiter, ERL_NIF_REGULAR_BOUND},
        {"release_rate_limiter", 1, erocksdb::ReleaseRateLimiter, ERL_NIF_REGULAR_BOUND},

        // env
        {"new_env", 1, erocksdb::NewEnv, ERL_NIF_REGULAR_BOUND},
        {"set_env_background_threads", 2, erocksdb::SetEnvBackgroundThreads, ERL_NIF_REGULAR_BOUND},
        {"set_env_background_threads", 3, erocksdb::SetEnvBackgroundThreads, ERL_NIF_REGULAR_BOUND},
        {"destroy_env", 1, erocksdb::DestroyEnv, ERL_NIF_REGULAR_BOUND},

        // SST File Manager
        {"new_sst_file_manager", 2, erocksdb::NewSstFileManager, ERL_NIF_REGULAR_BOUND},
        {"release_sst_file_manager", 1, erocksdb::ReleaseSstFileManager, ERL_NIF_REGULAR_BOUND},
        {"sst_file_manager_flag", 3, erocksdb::SstFileManagerFlag, ERL_NIF_REGULAR_BOUND},
        {"sst_file_manager_info", 1, erocksdb::SstFileManagerInfo, ERL_NIF_REGULAR_BOUND},
        {"sst_file_manager_info", 2, erocksdb::SstFileManagerInfo, ERL_NIF_REGULAR_BOUND},

        // Write Buffer Manager
        {"new_write_buffer_manager", 1, erocksdb::NewWriteBufferManager, ERL_NIF_REGULAR_BOUND},
        {"new_write_buffer_manager", 2, erocksdb::NewWriteBufferManager, ERL_NIF_REGULAR_BOUND},
        {"release_write_buffer_manager", 1, erocksdb::ReleaseWriteBufferManager, ERL_NIF_REGULAR_BOUND},
        {"write_buffer_manager_info", 1, erocksdb::WriteBufferManagerInfo, ERL_NIF_REGULAR_BOUND},
        {"write_buffer_manager_info", 2, erocksdb::WriteBufferManagerInfo, ERL_NIF_REGULAR_BOUND},

        // Statistics
        {"new_statistics", 0, erocksdb::NewStatistics, ERL_NIF_REGULAR_BOUND},
        {"set_stats_level", 2, erocksdb::SetStatsLevel, ERL_NIF_REGULAR_BOUND},
        {"statistics_info", 1, erocksdb::StatisticsInfo, ERL_NIF_REGULAR_BOUND},
        {"release_statistics", 1, erocksdb::ReleaseStatistics, ERL_NIF_REGULAR_BOUND},
        };

namespace erocksdb {

// Atoms (initialized in on_load)
// Related to Erlang
ERL_NIF_TERM ATOM_TRUE;
ERL_NIF_TERM ATOM_FALSE;
ERL_NIF_TERM ATOM_OK;
ERL_NIF_TERM ATOM_ERROR;
ERL_NIF_TERM ATOM_EINVAL;
ERL_NIF_TERM ATOM_BADARG;
ERL_NIF_TERM ATOM_NOT_FOUND;
ERL_NIF_TERM ATOM_CORRUPTION;
ERL_NIF_TERM ATOM_INC;
ERL_NIF_TERM ATOM_DEC;
ERL_NIF_TERM ATOM_UNKNOWN_STATUS_ERROR;
ERL_NIF_TERM ATOM_UNDEFINED;

// related to envs
ERL_NIF_TERM ATOM_DEFAULT;
ERL_NIF_TERM ATOM_MEMENV;

// related to cache
ERL_NIF_TERM ATOM_LRU;
ERL_NIF_TERM ATOM_CLOCK;
ERL_NIF_TERM ATOM_USAGE;
ERL_NIF_TERM ATOM_PINNED_USAGE;
ERL_NIF_TERM ATOM_CAPACITY;
ERL_NIF_TERM ATOM_STRICT_CAPACITY;
ERL_NIF_TERM ATOM_FLUSH_ONLY;
ERL_NIF_TERM ATOM_DISABLE;

// generic
ERL_NIF_TERM ATOM_DEFAULT_COLUMN_FAMILY;

// Related to CFOptions
ERL_NIF_TERM ATOM_BLOCK_CACHE_SIZE_MB_FOR_POINT_LOOKUP;
ERL_NIF_TERM ATOM_MEMTABLE_MEMORY_BUDGET;
ERL_NIF_TERM ATOM_WRITE_BUFFER_SIZE;
ERL_NIF_TERM ATOM_MAX_WRITE_BUFFER_NUMBER;
ERL_NIF_TERM ATOM_MIN_WRITE_BUFFER_NUMBER_TO_MERGE;
ERL_NIF_TERM ATOM_COMPRESSION;

// CFOptions blob
ERL_NIF_TERM ATOM_ENABLE_BLOB_FILES;
ERL_NIF_TERM ATOM_MIN_BLOB_SIZE;
ERL_NIF_TERM ATOM_BLOB_FILE_SIZE;
ERL_NIF_TERM ATOM_BLOB_COMPRESSION_TYPE;
ERL_NIF_TERM ATOM_ENABLE_BLOB_GC;
ERL_NIF_TERM ATOM_BLOB_GC_AGE_CUTOFF;
ERL_NIF_TERM ATOM_BLOB_GC_FORCE_THRESHOLD;
ERL_NIF_TERM ATOM_BLOB_COMPACTION_READAHEAD_SIZE;
ERL_NIF_TERM ATOM_BLOB_FILE_STARTING_LEVEL;
ERL_NIF_TERM ATOM_BLOB_CACHE;
ERL_NIF_TERM ATOM_PREPOLUATE_BLOB_CACHE;

// Related to CFOpCompressionOptions
ERL_NIF_TERM ATOM_BOTTOMMOST_COMPRESSION;
ERL_NIF_TERM ATOM_BOTTOMMOST_COMPRESSION_OPTS;
ERL_NIF_TERM ATOM_COMPRESSION_OPTS;
ERL_NIF_TERM ATOM_WINDOW_BITS;
ERL_NIF_TERM ATOM_LEVEL;
ERL_NIF_TERM ATOM_STRATEGY;
ERL_NIF_TERM ATOM_MAX_DICT_BYTES;
ERL_NIF_TERM ATOM_ZSTD_MAX_TRAIN_BYTES;

// FIFO's compaction style TTL
ERL_NIF_TERM ATOM_TTL;

ERL_NIF_TERM ATOM_NUM_LEVELS;
ERL_NIF_TERM ATOM_LEVEL0_FILE_NUM_COMPACTION_TRIGGER;
ERL_NIF_TERM ATOM_LEVEL0_SLOWDOWN_WRITES_TRIGGER;
ERL_NIF_TERM ATOM_LEVEL0_STOP_WRITES_TRIGGER;
ERL_NIF_TERM ATOM_TARGET_FILE_SIZE_BASE;
ERL_NIF_TERM ATOM_TARGET_FILE_SIZE_MULTIPLIER;
ERL_NIF_TERM ATOM_MAX_BYTES_FOR_LEVEL_BASE;
ERL_NIF_TERM ATOM_MAX_BYTES_FOR_LEVEL_MULTIPLIER;
ERL_NIF_TERM ATOM_MAX_COMPACTION_BYTES;
ERL_NIF_TERM ATOM_ARENA_BLOCK_SIZE;
ERL_NIF_TERM ATOM_DISABLE_AUTO_COMPACTIONS;
ERL_NIF_TERM ATOM_COMPACTION_STYLE;
ERL_NIF_TERM ATOM_COMPACTION_PRI;
ERL_NIF_TERM ATOM_FILTER_DELETES;
ERL_NIF_TERM ATOM_MAX_SEQUENTIAL_SKIP_IN_ITERATIONS;
ERL_NIF_TERM ATOM_INPLACE_UPDATE_SUPPORT;
ERL_NIF_TERM ATOM_INPLACE_UPDATE_NUM_LOCKS;
ERL_NIF_TERM ATOM_TABLE_FACTORY_BLOCK_CACHE_SIZE;
ERL_NIF_TERM ATOM_IN_MEMORY_MODE;
ERL_NIF_TERM ATOM_IN_MEMORY;
ERL_NIF_TERM ATOM_BLOCK_BASED_TABLE_OPTIONS;
ERL_NIF_TERM ATOM_COMPACTION_OPTIONS_FIFO;
ERL_NIF_TERM ATOM_LEVEL_COMPACTION_DYNAMIC_LEVEL_BYTES;
ERL_NIF_TERM ATOM_OPTIMIZE_FILTERS_FOR_HITS;
ERL_NIF_TERM ATOM_PREFIX_EXTRACTOR;

// Related to COMPACTION_OPTIONS_FIFO
ERL_NIF_TERM ATOM_ALLOW_COMPACTION;
ERL_NIF_TERM ATOM_AGE_FOR_WARM;
ERL_NIF_TERM ATOM_MAX_TABLE_FILES_SIZE;

// Related to DBOptions
ERL_NIF_TERM ATOM_TOTAL_THREADS;
ERL_NIF_TERM ATOM_CREATE_IF_MISSING;
ERL_NIF_TERM ATOM_CREATE_MISSING_COLUMN_FAMILIES;
ERL_NIF_TERM ATOM_ERROR_IF_EXISTS;
ERL_NIF_TERM ATOM_PARANOID_CHECKS;
ERL_NIF_TERM ATOM_MAX_OPEN_FILES;
ERL_NIF_TERM ATOM_MAX_TOTAL_WAL_SIZE;
ERL_NIF_TERM ATOM_USE_FSYNC;
ERL_NIF_TERM ATOM_DB_PATHS;
ERL_NIF_TERM ATOM_DB_LOG_DIR;
ERL_NIF_TERM ATOM_WAL_DIR;
ERL_NIF_TERM ATOM_DELETE_OBSOLETE_FILES_PERIOD_MICROS;
ERL_NIF_TERM ATOM_MAX_BACKGROUND_JOBS;
ERL_NIF_TERM ATOM_MAX_BACKGROUND_COMPACTIONS;
ERL_NIF_TERM ATOM_MAX_BACKGROUND_FLUSHES;
ERL_NIF_TERM ATOM_MAX_LOG_FILE_SIZE;
ERL_NIF_TERM ATOM_LOG_FILE_TIME_TO_ROLL;
ERL_NIF_TERM ATOM_KEEP_LOG_FILE_NUM;
ERL_NIF_TERM ATOM_MAX_MANIFEST_FILE_SIZE;
ERL_NIF_TERM ATOM_TABLE_CACHE_NUMSHARDBITS;
ERL_NIF_TERM ATOM_WAL_TTL_SECONDS;
ERL_NIF_TERM ATOM_WAL_SIZE_LIMIT_MB;
ERL_NIF_TERM ATOM_MANIFEST_PREALLOCATION_SIZE;
ERL_NIF_TERM ATOM_ALLOW_MMAP_READS;
ERL_NIF_TERM ATOM_ALLOW_MMAP_WRITES;
ERL_NIF_TERM ATOM_IS_FD_CLOSE_ON_EXEC;
ERL_NIF_TERM ATOM_STATS_DUMP_PERIOD_SEC;
ERL_NIF_TERM ATOM_ADVISE_RANDOM_ON_OPEN;
ERL_NIF_TERM ATOM_ACCESS_HINT;
ERL_NIF_TERM ATOM_COMPACTION_READAHEAD_SIZE;
ERL_NIF_TERM ATOM_USE_ADAPTIVE_MUTEX;
ERL_NIF_TERM ATOM_BYTES_PER_SYNC;
ERL_NIF_TERM ATOM_SKIP_STATS_UPDATE_ON_DB_OPEN;
ERL_NIF_TERM ATOM_WAL_RECOVERY_MODE;
ERL_NIF_TERM ATOM_ALLOW_CONCURRENT_MEMTABLE_WRITE;
ERL_NIF_TERM ATOM_ENABLE_WRITE_THREAD_ADAPTATIVE_YIELD;
ERL_NIF_TERM ATOM_DB_WRITE_BUFFER_SIZE;
ERL_NIF_TERM ATOM_RATE_LIMITER;
ERL_NIF_TERM ATOM_SST_FILE_MANAGER;
ERL_NIF_TERM ATOM_WRITE_BUFFER_MANAGER;
ERL_NIF_TERM ATOM_MAX_SUBCOMPACTIONS;
ERL_NIF_TERM ATOM_MANUAL_WAL_FLUSH;
ERL_NIF_TERM ATOM_ATOMIC_FLUSH;
ERL_NIF_TERM ATOM_USE_DIRECT_READS;
ERL_NIF_TERM ATOM_USE_DIRECT_IO_FOR_FLUSH_AND_COMPACTION;
ERL_NIF_TERM ATOM_ENABLE_PIPELINED_WRITE;
ERL_NIF_TERM ATOM_UNORDERED_WRITE;
ERL_NIF_TERM ATOM_TWO_WRITE_QUEUES;

// Related to BlockBasedTable Options
ERL_NIF_TERM ATOM_NO_BLOCK_CACHE;
ERL_NIF_TERM ATOM_BLOCK_CACHE;
ERL_NIF_TERM ATOM_BLOCK_SIZE;
ERL_NIF_TERM ATOM_BLOCK_CACHE_SIZE;
ERL_NIF_TERM ATOM_BLOOM_FILTER_POLICY;
ERL_NIF_TERM ATOM_FORMAT_VERSION;
ERL_NIF_TERM ATOM_CACHE_INDEX_AND_FILTER_BLOCKS;

// Related to ReadTier
ERL_NIF_TERM ATOM_READ_TIER;
ERL_NIF_TERM ATOM_READ_ALL_TIER;
ERL_NIF_TERM ATOM_BLOCK_CACHE_TIER;
ERL_NIF_TERM ATOM_PERSISTED_TIER;
ERL_NIF_TERM ATOM_MEMTABLE_TIER;

// Related to Read Options
ERL_NIF_TERM ATOM_VERIFY_CHECKSUMS;
ERL_NIF_TERM ATOM_FILL_CACHE;
ERL_NIF_TERM ATOM_ITERATE_UPPER_BOUND;
ERL_NIF_TERM ATOM_ITERATE_LOWER_BOUND;
ERL_NIF_TERM ATOM_TAILING;
ERL_NIF_TERM ATOM_TOTAL_ORDER_SEEK;
ERL_NIF_TERM ATOM_PREFIX_SAME_AS_START;
ERL_NIF_TERM ATOM_SNAPSHOT;
ERL_NIF_TERM ATOM_BAD_SNAPSHOT;

// Related to Write Options
ERL_NIF_TERM ATOM_SYNC;
ERL_NIF_TERM ATOM_DISABLE_WAL;
ERL_NIF_TERM ATOM_IGNORE_MISSING_COLUMN_FAMILIES;
ERL_NIF_TERM ATOM_NO_SLOWDOWN;
ERL_NIF_TERM ATOM_LOW_PRI;

// Related to Write Actions
ERL_NIF_TERM ATOM_CLEAR;
ERL_NIF_TERM ATOM_PUT;
ERL_NIF_TERM ATOM_MERGE;
ERL_NIF_TERM ATOM_DELETE;
ERL_NIF_TERM ATOM_SINGLE_DELETE;

// Related to CompactRangeOptions
ERL_NIF_TERM ATOM_EXCLUSIVE_MANUAL_COMPACTION;
ERL_NIF_TERM ATOM_CHANGE_LEVEL;
ERL_NIF_TERM ATOM_TARGET_LEVEL;
ERL_NIF_TERM ATOM_ALLOW_WRITE_STALL;

// Related to FlushOptions
ERL_NIF_TERM ATOM_WAIT;

// Related to Iterator Actions
ERL_NIF_TERM ATOM_FIRST;
ERL_NIF_TERM ATOM_LAST;
ERL_NIF_TERM ATOM_NEXT;
ERL_NIF_TERM ATOM_PREV;
ERL_NIF_TERM ATOM_SEEK_FOR_PREV;
ERL_NIF_TERM ATOM_SEEK;

// Related to Iterator Value to be retrieved
ERL_NIF_TERM ATOM_KEYS_ONLY;

// Related to Access Hint
ERL_NIF_TERM ATOM_ACCESS_HINT_NORMAL;
ERL_NIF_TERM ATOM_ACCESS_HINT_SEQUENTIAL;
ERL_NIF_TERM ATOM_ACCESS_HINT_WILLNEED;
ERL_NIF_TERM ATOM_ACCESS_HINT_NONE;

// Related to Compression Type
ERL_NIF_TERM ATOM_COMPRESSION_TYPE_SNAPPY;
ERL_NIF_TERM ATOM_COMPRESSION_TYPE_ZLIB;
ERL_NIF_TERM ATOM_COMPRESSION_TYPE_BZIP2;
ERL_NIF_TERM ATOM_COMPRESSION_TYPE_LZ4;
ERL_NIF_TERM ATOM_COMPRESSION_TYPE_LZ4H;
ERL_NIF_TERM ATOM_COMPRESSION_TYPE_ZSTD;
ERL_NIF_TERM ATOM_COMPRESSION_TYPE_NONE;

// Related to Compaction Style
ERL_NIF_TERM ATOM_COMPACTION_STYLE_LEVEL;
ERL_NIF_TERM ATOM_COMPACTION_STYLE_UNIVERSAL;
ERL_NIF_TERM ATOM_COMPACTION_STYLE_FIFO;
ERL_NIF_TERM ATOM_COMPACTION_STYLE_NONE;

// Related to Compaction Priority
ERL_NIF_TERM ATOM_COMPACTION_PRI_COMPENSATED_SIZE;
ERL_NIF_TERM ATOM_COMPACTION_PRI_OLDEST_LARGEST_SEQ_FIRST;
ERL_NIF_TERM ATOM_COMPACTION_PRI_OLDEST_SMALLEST_SEQ_FIRST;

// Related to WAL Recovery Mode
ERL_NIF_TERM ATOM_WAL_TOLERATE_CORRUPTED_TAIL_RECORDS;
ERL_NIF_TERM ATOM_WAL_ABSOLUTE_CONSISTENCY;
ERL_NIF_TERM ATOM_WAL_POINT_IN_TIME_RECOVERY;
ERL_NIF_TERM ATOM_WAL_SKIP_ANY_CORRUPTED_RECORDS;

// Related to Error Codes
ERL_NIF_TERM ATOM_ERROR_DB_OPEN;
ERL_NIF_TERM ATOM_ERROR_DB_PUT;
ERL_NIF_TERM ATOM_ERROR_DB_DELETE;
ERL_NIF_TERM ATOM_ERROR_DB_WRITE;
ERL_NIF_TERM ATOM_ERROR_DB_DESTROY;
ERL_NIF_TERM ATOM_ERROR_DB_REPAIR;
ERL_NIF_TERM ATOM_BAD_WRITE_ACTION;
ERL_NIF_TERM ATOM_KEEP_RESOURCE_FAILED;
ERL_NIF_TERM ATOM_ITERATOR_CLOSED;
ERL_NIF_TERM ATOM_INVALID_ITERATOR;
ERL_NIF_TERM ATOM_ERROR_BACKUP_ENGINE_OPEN;
ERL_NIF_TERM ATOM_ERROR_INCOMPLETE;

// Related to NIF initialize parameters
ERL_NIF_TERM ATOM_WRITE_THREADS;

ERL_NIF_TERM ATOM_ENV;
ERL_NIF_TERM ATOM_PRIORITY_HIGH;
ERL_NIF_TERM ATOM_PRIORITY_LOW;


// backup info
ERL_NIF_TERM ATOM_BACKUP_INFO_ID;
ERL_NIF_TERM ATOM_BACKUP_INFO_TIMESTAMP;
ERL_NIF_TERM ATOM_BACKUP_INFO_SIZE;
ERL_NIF_TERM ATOM_BACKUP_INFO_NUMBER_FILES;


ERL_NIF_TERM ATOM_MERGE_OPERATOR;
ERL_NIF_TERM ATOM_ERLANG_MERGE_OPERATOR;
ERL_NIF_TERM ATOM_BITSET_MERGE_OPERATOR;
ERL_NIF_TERM ATOM_COUNTER_MERGE_OPERATOR;

ERL_NIF_TERM ATOM_MERGE_INT_ADD;
ERL_NIF_TERM ATOM_MERGE_LIST_APPEND;
ERL_NIF_TERM ATOM_MERGE_LIST_SUBSTRACT;
ERL_NIF_TERM ATOM_MERGE_LIST_SET;
ERL_NIF_TERM ATOM_MERGE_LIST_DELETE;
ERL_NIF_TERM ATOM_MERGE_LIST_INSERT;
ERL_NIF_TERM ATOM_MERGE_BINARY_APPEND;
ERL_NIF_TERM ATOM_MERGE_BINARY_REPLACE;
ERL_NIF_TERM ATOM_MERGE_BINARY_INSERT;
ERL_NIF_TERM ATOM_MERGE_BINARY_ERASE;

ERL_NIF_TERM ATOM_FIXED_PREFIX_TRANSFORM;
ERL_NIF_TERM ATOM_CAPPED_PREFIX_TRANSFORM;

ERL_NIF_TERM ATOM_COMPARATOR;
ERL_NIF_TERM ATOM_BYTEWISE_COMPARATOR;
ERL_NIF_TERM ATOM_REVERSE_BYTEWISE_COMPARATOR;

// range

ERL_NIF_TERM ATOM_NONE;
ERL_NIF_TERM ATOM_INCLUDE_MEMTABLES;
ERL_NIF_TERM ATOM_INCLUDE_FILES;
ERL_NIF_TERM ATOM_INCLUDE_BOTH;

// write buffer manager
ERL_NIF_TERM ATOM_ENABLED;
ERL_NIF_TERM ATOM_BUFFER_SIZE;
ERL_NIF_TERM ATOM_MUTABLE_MEMTABLE_MEMORY_USAGE;
ERL_NIF_TERM ATOM_MEMORY_USAGE;

// sst file manager

ERL_NIF_TERM ATOM_DELETE_RATE_BYTES_PER_SEC;
ERL_NIF_TERM ATOM_MAX_TRASH_DB_RATIO;
ERL_NIF_TERM ATOM_BYTES_MAX_DELETE_CHUNK;
ERL_NIF_TERM ATOM_MAX_ALLOWED_SPACE_USAGE;
ERL_NIF_TERM ATOM_COMPACTION_BUFFER_SIZE;
ERL_NIF_TERM ATOM_IS_MAX_ALLOWED_SPACE_REACHED;
ERL_NIF_TERM ATOM_MAX_ALLOWED_SPACE_REACHED_INCLUDING_COMPACTIONS;
ERL_NIF_TERM ATOM_TOTAL_SIZE;
ERL_NIF_TERM ATOM_TOTAL_TRASH_SIZE;

// statistics
ERL_NIF_TERM ATOM_STATISTICS;
ERL_NIF_TERM ATOM_STATS_DISABLE_ALL;
ERL_NIF_TERM ATOM_STATS_EXCEPT_TICKERS;
ERL_NIF_TERM ATOM_STATS_EXCEPT_HISTOGRAM_OR_TIMERS;
ERL_NIF_TERM ATOM_STATS_EXCEPT_TIMERS;
ERL_NIF_TERM ATOM_STATS_EXCEPT_DETAILED_TIMERS;
ERL_NIF_TERM ATOM_STATS_EXCEPT_TIME_FOR_MUTEX;
ERL_NIF_TERM ATOM_STATS_ALL;
ERL_NIF_TERM ATOM_STATS_LEVEL;

}   // namespace erocksdb


using std::nothrow;

static void on_unload(ErlNifEnv * /*env*/, void * /*priv_data*/)
{
}

static int on_upgrade(ErlNifEnv* /*env*/, void** priv_data, void** old_priv_data, ERL_NIF_TERM /*load_info*/)
{
    /* Convert the private data to the new version. */
    *priv_data = *old_priv_data;
    return 0;
}

static int on_load(ErlNifEnv* env, void** /*priv_data*/, ERL_NIF_TERM /*load_info*/)
try
{
  rocksdb::Env::Default();

  // inform erlang of our two resource types
  erocksdb::ManagedEnv::CreateEnvType(env);
  erocksdb::DbObject::CreateDbObjectType(env);
  erocksdb::ColumnFamilyObject::CreateColumnFamilyObjectType(env);
  erocksdb::ItrObject::CreateItrObjectType(env);
  erocksdb::SnapshotObject::CreateSnapshotObjectType(env);
  erocksdb::CreateBatchType(env);
  erocksdb::TransactionObject::CreateTransactionObjectType(env);
  erocksdb::TLogItrObject::CreateTLogItrObjectType(env);
  erocksdb::BackupEngineObject::CreateBackupEngineObjectType(env);
  erocksdb::Cache::CreateCacheType(env);
  erocksdb::Statistics::CreateStatisticsType(env);
  erocksdb::RateLimiter::CreateRateLimiterType(env);
  erocksdb::SstFileManager::CreateSstFileManagerType(env);
  erocksdb::WriteBufferManager::CreateWriteBufferManagerType(env);

  // must initialize atoms before processing options
#define ATOM(Id, Value) { Id = enif_make_atom(env, Value); }
  // Related to Erlang
  ATOM(erocksdb::ATOM_TRUE, "true");
  ATOM(erocksdb::ATOM_FALSE, "false");
  ATOM(erocksdb::ATOM_OK, "ok");
  ATOM(erocksdb::ATOM_ERROR, "error");
  ATOM(erocksdb::ATOM_EINVAL, "einval");
  ATOM(erocksdb::ATOM_BADARG, "badarg");
  ATOM(erocksdb::ATOM_NOT_FOUND, "not_found");
  ATOM(erocksdb::ATOM_CORRUPTION, "corruption");
  ATOM(erocksdb::ATOM_INC, "inc");
  ATOM(erocksdb::ATOM_DEC, "dec");
  ATOM(erocksdb::ATOM_UNKNOWN_STATUS_ERROR, "unknown_status");
  ATOM(erocksdb::ATOM_UNDEFINED, "undefined");

  ATOM(erocksdb::ATOM_DEFAULT, "default");
  ATOM(erocksdb::ATOM_MEMENV, "memenv");

  ATOM(erocksdb::ATOM_LRU, "lru");
  ATOM(erocksdb::ATOM_CLOCK, "clock");
  ATOM(erocksdb::ATOM_USAGE, "usage");
  ATOM(erocksdb::ATOM_PINNED_USAGE, "pinned_usage");
  ATOM(erocksdb::ATOM_CAPACITY, "capacity");
  ATOM(erocksdb::ATOM_STRICT_CAPACITY, "strict_capacity");
  ATOM(erocksdb::ATOM_FLUSH_ONLY, "flush_only");
  ATOM(erocksdb::ATOM_DISABLE, "disable");

  ATOM(erocksdb::ATOM_DEFAULT_COLUMN_FAMILY, "default_column_family");

  // Related to CFOptions
  ATOM(erocksdb::ATOM_BLOCK_CACHE_SIZE_MB_FOR_POINT_LOOKUP, "block_cache_size_mb_for_point_lookup");
  ATOM(erocksdb::ATOM_MEMTABLE_MEMORY_BUDGET, "memtable_memory_budget");
  ATOM(erocksdb::ATOM_WRITE_BUFFER_SIZE, "write_buffer_size");
  ATOM(erocksdb::ATOM_MAX_WRITE_BUFFER_NUMBER, "max_write_buffer_number");
  ATOM(erocksdb::ATOM_MIN_WRITE_BUFFER_NUMBER_TO_MERGE, "min_write_buffer_number_to_merge");
  ATOM(erocksdb::ATOM_COMPRESSION, "compression");

  ATOM(erocksdb::ATOM_ENABLE_BLOB_FILES, "enable_blob_files");
  ATOM(erocksdb::ATOM_MIN_BLOB_SIZE, "min_blob_size");
  ATOM(erocksdb::ATOM_BLOB_FILE_SIZE, "blob_file_size");
  ATOM(erocksdb::ATOM_BLOB_COMPRESSION_TYPE, "blob_compression_type");
  ATOM(erocksdb::ATOM_ENABLE_BLOB_GC, "enable_blob_garbage_collection");
  ATOM(erocksdb::ATOM_BLOB_GC_AGE_CUTOFF, "blob_garbage_collection_age_cutoff");
  ATOM(erocksdb::ATOM_BLOB_GC_FORCE_THRESHOLD, "blob_garbage_collection_force_threshold");
  ATOM(erocksdb::ATOM_BLOB_COMPACTION_READAHEAD_SIZE, "blob_compaction_readahead_size");
  ATOM(erocksdb::ATOM_BLOB_FILE_STARTING_LEVEL, "blob_file_starting_level");
  ATOM(erocksdb::ATOM_BLOB_CACHE, "blob_cache");
  ATOM(erocksdb::ATOM_PREPOLUATE_BLOB_CACHE, "prepopulate_blob_cache");
  ATOM(erocksdb::ATOM_BOTTOMMOST_COMPRESSION, "bottommost_compression");
  ATOM(erocksdb::ATOM_BOTTOMMOST_COMPRESSION_OPTS, "bottommost_compression_opts");
  ATOM(erocksdb::ATOM_COMPRESSION_OPTS, "compression_opts");
  ATOM(erocksdb::ATOM_WINDOW_BITS, "window_bits");
  ATOM(erocksdb::ATOM_LEVEL, "level");
  ATOM(erocksdb::ATOM_STRATEGY, "strategy");
  ATOM(erocksdb::ATOM_MAX_DICT_BYTES, "max_dict_bytes");
  ATOM(erocksdb::ATOM_ZSTD_MAX_TRAIN_BYTES, "zstd_max_train_bytes");

  ATOM(erocksdb::ATOM_TTL, "ttl");

  ATOM(erocksdb::ATOM_NUM_LEVELS, "num_levels");
  ATOM(erocksdb::ATOM_LEVEL0_FILE_NUM_COMPACTION_TRIGGER, "level0_file_num_compaction_trigger");
  ATOM(erocksdb::ATOM_LEVEL0_SLOWDOWN_WRITES_TRIGGER, "level0_slowdown_writes_trigger");
  ATOM(erocksdb::ATOM_LEVEL0_STOP_WRITES_TRIGGER, "level0_stop_writes_trigger");
  ATOM(erocksdb::ATOM_TARGET_FILE_SIZE_BASE, "target_file_size_base");
  ATOM(erocksdb::ATOM_TARGET_FILE_SIZE_MULTIPLIER, "target_file_size_multiplier");
  ATOM(erocksdb::ATOM_MAX_BYTES_FOR_LEVEL_BASE, "max_bytes_for_level_base");
  ATOM(erocksdb::ATOM_MAX_BYTES_FOR_LEVEL_MULTIPLIER, "max_bytes_for_level_multiplier");
  ATOM(erocksdb::ATOM_MAX_COMPACTION_BYTES, "max_compaction_bytes");
  ATOM(erocksdb::ATOM_ARENA_BLOCK_SIZE, "arena_block_size");
  ATOM(erocksdb::ATOM_DISABLE_AUTO_COMPACTIONS, "disable_auto_compactions");
  ATOM(erocksdb::ATOM_COMPACTION_STYLE, "compaction_style");
  ATOM(erocksdb::ATOM_COMPACTION_PRI, "compaction_pri");
  ATOM(erocksdb::ATOM_FILTER_DELETES, "filter_deletes");
  ATOM(erocksdb::ATOM_MAX_SEQUENTIAL_SKIP_IN_ITERATIONS, "max_sequential_skip_in_iterations");
  ATOM(erocksdb::ATOM_INPLACE_UPDATE_SUPPORT, "inplace_update_support");
  ATOM(erocksdb::ATOM_INPLACE_UPDATE_NUM_LOCKS, "inplace_update_num_locks");
  ATOM(erocksdb::ATOM_TABLE_FACTORY_BLOCK_CACHE_SIZE, "table_factory_block_cache_size");
  ATOM(erocksdb::ATOM_IN_MEMORY_MODE, "in_memory_mode");
  ATOM(erocksdb::ATOM_IN_MEMORY, "in_memory");
  ATOM(erocksdb::ATOM_BLOCK_BASED_TABLE_OPTIONS, "block_based_table_options");
  ATOM(erocksdb::ATOM_COMPACTION_OPTIONS_FIFO, "compaction_options_fifo");
  ATOM(erocksdb::ATOM_LEVEL_COMPACTION_DYNAMIC_LEVEL_BYTES, "level_compaction_dynamic_level_bytes");
  ATOM(erocksdb::ATOM_OPTIMIZE_FILTERS_FOR_HITS, "optimize_filters_for_hits");
  ATOM(erocksdb::ATOM_PREFIX_EXTRACTOR, "prefix_extractor");

  // Related to COMPACTION_OPTIONS_FIFO
  ATOM(erocksdb::ATOM_ALLOW_COMPACTION, "allow_compaction");
  ATOM(erocksdb::ATOM_AGE_FOR_WARM, "age_for_warm");
  ATOM(erocksdb::ATOM_MAX_TABLE_FILES_SIZE, "max_table_files_size");

  // Related to DBOptions
  ATOM(erocksdb::ATOM_TOTAL_THREADS, "total_threads");
  ATOM(erocksdb::ATOM_CREATE_IF_MISSING, "create_if_missing");
  ATOM(erocksdb::ATOM_CREATE_MISSING_COLUMN_FAMILIES, "create_missing_column_families");
  ATOM(erocksdb::ATOM_ERROR_IF_EXISTS, "error_if_exists");
  ATOM(erocksdb::ATOM_PARANOID_CHECKS, "paranoid_checks");
  ATOM(erocksdb::ATOM_MAX_OPEN_FILES, "max_open_files");
  ATOM(erocksdb::ATOM_MAX_TOTAL_WAL_SIZE, "max_total_wal_size");
  ATOM(erocksdb::ATOM_USE_FSYNC, "use_fsync");
  ATOM(erocksdb::ATOM_DB_PATHS, "db_paths");
  ATOM(erocksdb::ATOM_DB_LOG_DIR, "db_log_dir");
  ATOM(erocksdb::ATOM_WAL_DIR, "wal_dir");
  ATOM(erocksdb::ATOM_DELETE_OBSOLETE_FILES_PERIOD_MICROS, "delete_obsolete_files_period_micros");
  ATOM(erocksdb::ATOM_MAX_BACKGROUND_JOBS, "max_background_jobs");
  ATOM(erocksdb::ATOM_MAX_BACKGROUND_COMPACTIONS, "max_background_compactions");
  ATOM(erocksdb::ATOM_MAX_BACKGROUND_FLUSHES, "max_background_flushes");
  ATOM(erocksdb::ATOM_MAX_LOG_FILE_SIZE, "max_log_file_size");
  ATOM(erocksdb::ATOM_LOG_FILE_TIME_TO_ROLL, "log_file_time_to_roll");
  ATOM(erocksdb::ATOM_KEEP_LOG_FILE_NUM, "keep_log_file_num");
  ATOM(erocksdb::ATOM_MAX_MANIFEST_FILE_SIZE, "max_manifest_file_size");
  ATOM(erocksdb::ATOM_TABLE_CACHE_NUMSHARDBITS, "table_cache_numshardbits");
  ATOM(erocksdb::ATOM_WAL_TTL_SECONDS, "wal_ttl_seconds");
  ATOM(erocksdb::ATOM_WAL_SIZE_LIMIT_MB, "wal_size_limit_mb");
  ATOM(erocksdb::ATOM_MANIFEST_PREALLOCATION_SIZE, "manifest_preallocation_size");
  ATOM(erocksdb::ATOM_ALLOW_MMAP_READS, "allow_mmap_reads");
  ATOM(erocksdb::ATOM_ALLOW_MMAP_WRITES, "allow_mmap_writes");
  ATOM(erocksdb::ATOM_IS_FD_CLOSE_ON_EXEC, "is_fd_close_on_exec");
  ATOM(erocksdb::ATOM_STATS_DUMP_PERIOD_SEC, "stats_dump_period_sec");
  ATOM(erocksdb::ATOM_ADVISE_RANDOM_ON_OPEN, "advise_random_on_open");
  ATOM(erocksdb::ATOM_ACCESS_HINT, "access_hint");
  ATOM(erocksdb::ATOM_COMPACTION_READAHEAD_SIZE, "compaction_readahead_size");
  ATOM(erocksdb::ATOM_USE_ADAPTIVE_MUTEX, "use_adaptive_mutex");
  ATOM(erocksdb::ATOM_BYTES_PER_SYNC, "bytes_per_sync");
  ATOM(erocksdb::ATOM_SKIP_STATS_UPDATE_ON_DB_OPEN, "skip_stats_update_on_db_open");
  ATOM(erocksdb::ATOM_WAL_RECOVERY_MODE, "wal_recovery_mode");
  ATOM(erocksdb::ATOM_ALLOW_CONCURRENT_MEMTABLE_WRITE, "allow_concurrent_memtable_write");
  ATOM(erocksdb::ATOM_ENABLE_WRITE_THREAD_ADAPTATIVE_YIELD, "enable_write_thread_adaptive_yield");
  ATOM(erocksdb::ATOM_DB_WRITE_BUFFER_SIZE, "db_write_buffer_size");
  ATOM(erocksdb::ATOM_RATE_LIMITER, "rate_limiter");
  ATOM(erocksdb::ATOM_SST_FILE_MANAGER, "sst_file_manager");
  ATOM(erocksdb::ATOM_WRITE_BUFFER_MANAGER, "write_buffer_manager");
  ATOM(erocksdb::ATOM_MAX_SUBCOMPACTIONS, "max_subcompactions");
  ATOM(erocksdb::ATOM_MANUAL_WAL_FLUSH, "manual_wal_flush");
  ATOM(erocksdb::ATOM_ATOMIC_FLUSH, "atomic_flush");
  ATOM(erocksdb::ATOM_USE_DIRECT_READS, "use_direct_reads");
  ATOM(erocksdb::ATOM_USE_DIRECT_IO_FOR_FLUSH_AND_COMPACTION, "use_direct_io_for_flush_and_compaction");
  ATOM(erocksdb::ATOM_ENABLE_PIPELINED_WRITE, "enable_pipelined_write");
  ATOM(erocksdb::ATOM_UNORDERED_WRITE, "unordered_write");
  ATOM(erocksdb::ATOM_TWO_WRITE_QUEUES, "two_write_queues");

  // Related to BlockBasedTable Options
  ATOM(erocksdb::ATOM_NO_BLOCK_CACHE, "no_block_cache");
  ATOM(erocksdb::ATOM_BLOCK_CACHE, "block_cache");

  ATOM(erocksdb::ATOM_BLOCK_SIZE, "block_size");
  ATOM(erocksdb::ATOM_BLOCK_CACHE_SIZE, "block_cache_size");
  ATOM(erocksdb::ATOM_BLOOM_FILTER_POLICY, "bloom_filter_policy");
  ATOM(erocksdb::ATOM_FORMAT_VERSION, "format_version");
  ATOM(erocksdb::ATOM_CACHE_INDEX_AND_FILTER_BLOCKS, "cache_index_and_filter_blocks");

  // Related to ReadTier
  ATOM(erocksdb::ATOM_READ_TIER, "read_tier");
  ATOM(erocksdb::ATOM_READ_ALL_TIER, "read_all_tier");
  ATOM(erocksdb::ATOM_BLOCK_CACHE_TIER, "block_cache_tier");
  ATOM(erocksdb::ATOM_PERSISTED_TIER, "persisted_tier");
  ATOM(erocksdb::ATOM_MEMTABLE_TIER, "memtable_tier");

  // Related to Read Options
  ATOM(erocksdb::ATOM_VERIFY_CHECKSUMS, "verify_checksums");
  ATOM(erocksdb::ATOM_FILL_CACHE,"fill_cache");
  ATOM(erocksdb::ATOM_ITERATE_UPPER_BOUND,"iterate_upper_bound");
  ATOM(erocksdb::ATOM_ITERATE_LOWER_BOUND,"iterate_lower_bound");
  ATOM(erocksdb::ATOM_TAILING,"tailing");
  ATOM(erocksdb::ATOM_TOTAL_ORDER_SEEK,"total_order_seek");
  ATOM(erocksdb::ATOM_PREFIX_SAME_AS_START,"prefix_same_as_start");
  ATOM(erocksdb::ATOM_SNAPSHOT, "snapshot");
  ATOM(erocksdb::ATOM_BAD_SNAPSHOT, "bad_snapshot");

  // Related to Write Options
  ATOM(erocksdb::ATOM_SYNC, "sync");
  ATOM(erocksdb::ATOM_DISABLE_WAL, "disable_wal");
  ATOM(erocksdb::ATOM_IGNORE_MISSING_COLUMN_FAMILIES, "ignore_missing_column_families");
  ATOM(erocksdb::ATOM_NO_SLOWDOWN, "no_slowdown");
  ATOM(erocksdb::ATOM_LOW_PRI, "low_pri");

  // Related to Write Options
  ATOM(erocksdb::ATOM_CLEAR, "clear");
  ATOM(erocksdb::ATOM_PUT, "put");
  ATOM(erocksdb::ATOM_MERGE, "merge");
  ATOM(erocksdb::ATOM_DELETE, "delete");
  ATOM(erocksdb::ATOM_SINGLE_DELETE, "single_delete");

  // Related to CompactRangeOptions
  ATOM(erocksdb::ATOM_EXCLUSIVE_MANUAL_COMPACTION, "exclusive_manual_compaction");
  ATOM(erocksdb::ATOM_CHANGE_LEVEL, "change_level");
  ATOM(erocksdb::ATOM_TARGET_LEVEL, "target_level");
  ATOM(erocksdb::ATOM_ALLOW_WRITE_STALL, "allow_write_stall");

  // related to FlushOptions
  ATOM(erocksdb::ATOM_WAIT, "wait");

  // Related to Iterator Options
  ATOM(erocksdb::ATOM_FIRST, "first");
  ATOM(erocksdb::ATOM_LAST, "last");
  ATOM(erocksdb::ATOM_NEXT, "next");
  ATOM(erocksdb::ATOM_PREV, "prev");
  ATOM(erocksdb::ATOM_SEEK_FOR_PREV, "seek_for_prev");
  ATOM(erocksdb::ATOM_SEEK, "seek");

  // Related to Iterator Value to be retrieved
  ATOM(erocksdb::ATOM_KEYS_ONLY, "keys_only");

  // Related to Access Hint
  ATOM(erocksdb::ATOM_ACCESS_HINT_NORMAL,"normal");
  ATOM(erocksdb::ATOM_ACCESS_HINT_SEQUENTIAL,"sequential");
  ATOM(erocksdb::ATOM_ACCESS_HINT_WILLNEED,"willneed");
  ATOM(erocksdb::ATOM_ACCESS_HINT_NONE,"none");

  // Related to Compression Type
  ATOM(erocksdb::ATOM_COMPRESSION_TYPE_SNAPPY, "snappy");
  ATOM(erocksdb::ATOM_COMPRESSION_TYPE_ZLIB, "zlib");
  ATOM(erocksdb::ATOM_COMPRESSION_TYPE_BZIP2, "bzip2");
  ATOM(erocksdb::ATOM_COMPRESSION_TYPE_LZ4, "lz4");
  ATOM(erocksdb::ATOM_COMPRESSION_TYPE_LZ4H, "lz4h");
  ATOM(erocksdb::ATOM_COMPRESSION_TYPE_ZSTD, "zstd");
  ATOM(erocksdb::ATOM_COMPRESSION_TYPE_NONE, "none");

  // Related to Compaction Style
  ATOM(erocksdb::ATOM_COMPACTION_STYLE_LEVEL, "level");
  ATOM(erocksdb::ATOM_COMPACTION_STYLE_UNIVERSAL, "universal");
  ATOM(erocksdb::ATOM_COMPACTION_STYLE_FIFO, "fifo");
  ATOM(erocksdb::ATOM_COMPACTION_STYLE_NONE, "none");

  // Related to Compaction Priority
  ATOM(erocksdb::ATOM_COMPACTION_PRI_COMPENSATED_SIZE, "compensated_size");
  ATOM(erocksdb::ATOM_COMPACTION_PRI_OLDEST_LARGEST_SEQ_FIRST, "oldest_largest_seq_first");
  ATOM(erocksdb::ATOM_COMPACTION_PRI_OLDEST_SMALLEST_SEQ_FIRST, "oldest_smallest_seq_first");

  // Related to WAL Recovery Mode
  ATOM(erocksdb::ATOM_WAL_TOLERATE_CORRUPTED_TAIL_RECORDS, "tolerate_corrupted_tail_records");
  ATOM(erocksdb::ATOM_WAL_ABSOLUTE_CONSISTENCY, "absolute_consistency");
  ATOM(erocksdb::ATOM_WAL_POINT_IN_TIME_RECOVERY, "point_in_time_recovery");
  ATOM(erocksdb::ATOM_WAL_SKIP_ANY_CORRUPTED_RECORDS, "skip_any_corrupted_records");

  // Related to Error Codes
  ATOM(erocksdb::ATOM_ERROR_DB_OPEN,"db_open");
  ATOM(erocksdb::ATOM_ERROR_DB_PUT, "db_put");
  ATOM(erocksdb::ATOM_ERROR_DB_DELETE, "db_delete");
  ATOM(erocksdb::ATOM_ERROR_DB_WRITE, "db_write");
  ATOM(erocksdb::ATOM_ERROR_DB_DESTROY, "error_db_destroy");
  ATOM(erocksdb::ATOM_ERROR_DB_REPAIR, "error_db_repair");
  ATOM(erocksdb::ATOM_BAD_WRITE_ACTION, "bad_write_action");
  ATOM(erocksdb::ATOM_KEEP_RESOURCE_FAILED, "keep_resource_failed");
  ATOM(erocksdb::ATOM_ITERATOR_CLOSED, "iterator_closed");
  ATOM(erocksdb::ATOM_INVALID_ITERATOR, "invalid_iterator");
  ATOM(erocksdb::ATOM_ERROR_BACKUP_ENGINE_OPEN, "backup_engine_open");
  ATOM(erocksdb::ATOM_ERROR_INCOMPLETE, "incomplete");

  // Related to NIF initialize parameters
  ATOM(erocksdb::ATOM_WRITE_THREADS, "write_threads");

  ATOM(erocksdb::ATOM_PRIORITY_HIGH, "priority_high");
  ATOM(erocksdb::ATOM_PRIORITY_LOW, "priority_low");
  ATOM(erocksdb::ATOM_ENV, "env");

  // backup info
  ATOM(erocksdb::ATOM_BACKUP_INFO_ID, "backup_id");
  ATOM(erocksdb::ATOM_BACKUP_INFO_TIMESTAMP, "timestamp");
  ATOM(erocksdb::ATOM_BACKUP_INFO_SIZE, "size");
  ATOM(erocksdb::ATOM_BACKUP_INFO_NUMBER_FILES, "number_files");

    // Related to Merge OPs
  ATOM(erocksdb::ATOM_MERGE_OPERATOR, "merge_operator");
  ATOM(erocksdb::ATOM_ERLANG_MERGE_OPERATOR, "erlang_merge_operator");
  ATOM(erocksdb::ATOM_BITSET_MERGE_OPERATOR, "bitset_merge_operator");
  ATOM(erocksdb::ATOM_COUNTER_MERGE_OPERATOR, "counter_merge_operator");

  // erlang merge ops
  ATOM(erocksdb::ATOM_MERGE_INT_ADD, "int_add");
  ATOM(erocksdb::ATOM_MERGE_LIST_APPEND, "list_append");
  ATOM(erocksdb::ATOM_MERGE_LIST_SUBSTRACT, "list_substract");
  ATOM(erocksdb::ATOM_MERGE_LIST_SET, "list_set");
  ATOM(erocksdb::ATOM_MERGE_LIST_DELETE, "list_delete");
  ATOM(erocksdb::ATOM_MERGE_LIST_INSERT, "list_insert");
  ATOM(erocksdb::ATOM_MERGE_BINARY_APPEND, "binary_append");
  ATOM(erocksdb::ATOM_MERGE_BINARY_REPLACE, "binary_replace");
  ATOM(erocksdb::ATOM_MERGE_BINARY_INSERT, "binary_insert");
  ATOM(erocksdb::ATOM_MERGE_BINARY_ERASE, "binary_erase");

  // prefix extractor
  ATOM(erocksdb::ATOM_FIXED_PREFIX_TRANSFORM, "fixed_prefix_transform");
  ATOM(erocksdb::ATOM_CAPPED_PREFIX_TRANSFORM, "capped_prefix_transform");

  // comparator
  ATOM(erocksdb::ATOM_COMPARATOR, "comparator");
  ATOM(erocksdb::ATOM_BYTEWISE_COMPARATOR, "bytewise_comparator");
  ATOM(erocksdb::ATOM_REVERSE_BYTEWISE_COMPARATOR, "reverse_bytewise_comparator");

  // range
  ATOM(erocksdb::ATOM_NONE, "none");
  ATOM(erocksdb::ATOM_INCLUDE_MEMTABLES, "include_memtables");
  ATOM(erocksdb::ATOM_INCLUDE_FILES, "include_files");
  ATOM(erocksdb::ATOM_INCLUDE_BOTH, "include_both");

  // write buffer manager
  ATOM(erocksdb::ATOM_ENABLED, "enabled");
  ATOM(erocksdb::ATOM_BUFFER_SIZE, "buffer_size");
  ATOM(erocksdb::ATOM_MUTABLE_MEMTABLE_MEMORY_USAGE, "mutable_memtable_memory_usage");
  ATOM(erocksdb::ATOM_MEMORY_USAGE, "memory_usage");

  // sst file manager
  ATOM(erocksdb::ATOM_DELETE_RATE_BYTES_PER_SEC, "delete_rate_bytes_per_sec");
  ATOM(erocksdb::ATOM_MAX_TRASH_DB_RATIO, "max_trash_db_ratio");
  ATOM(erocksdb::ATOM_BYTES_MAX_DELETE_CHUNK, "bytes_max_delete_chunk");
  ATOM(erocksdb::ATOM_MAX_ALLOWED_SPACE_USAGE, "max_allowed_space_usage");
  ATOM(erocksdb::ATOM_COMPACTION_BUFFER_SIZE, "compaction_buffer_size");
  ATOM(erocksdb::ATOM_IS_MAX_ALLOWED_SPACE_REACHED, "is_max_allowed_space_reached");
  ATOM(erocksdb::ATOM_MAX_ALLOWED_SPACE_REACHED_INCLUDING_COMPACTIONS, "max_allowed_space_reached_including_compactions");
  ATOM(erocksdb::ATOM_TOTAL_SIZE, "total_size");
  ATOM(erocksdb::ATOM_TOTAL_TRASH_SIZE, "total_trash_size");

  // statistics
  ATOM(erocksdb::ATOM_STATISTICS, "statistics");
  ATOM(erocksdb::ATOM_STATS_DISABLE_ALL, "stats_disable_all");
  ATOM(erocksdb::ATOM_STATS_EXCEPT_TICKERS, "stats_except_tickers");
  ATOM(erocksdb::ATOM_STATS_EXCEPT_HISTOGRAM_OR_TIMERS, "stats_except_histogram_or_timers");
  ATOM(erocksdb::ATOM_STATS_EXCEPT_TIMERS, "stats_except_timers");
  ATOM(erocksdb::ATOM_STATS_EXCEPT_DETAILED_TIMERS, "stats_except_detailed_timers");
  ATOM(erocksdb::ATOM_STATS_EXCEPT_TIME_FOR_MUTEX, "stats_except_time_for_mutex");
  ATOM(erocksdb::ATOM_STATS_ALL, "stats_all");
  ATOM(erocksdb::ATOM_STATS_LEVEL, "stats_level");

#undef ATOM

return 0;
}
catch(std::exception& )
{
    /* Refuse to load the NIF module (I see no way right now to return a more specific exception
    or log extra information): */
    return -1;
}
catch(...)
{
    return -1;
}

extern "C" {
    ERL_NIF_INIT(rocksdb, nif_funcs, &on_load, NULL, &on_upgrade, &on_unload)
}
