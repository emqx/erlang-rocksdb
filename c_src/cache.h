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
//

#include <mutex>
#include <memory>

#include "rocksdb/version.h"
#if ROCKSDB_MAJOR < 8
#include "rocksdb/cache.h"
#else
#include "rocksdb/advanced_cache.h"
#endif

#include "erl_nif.h"

namespace erocksdb {

#if ROCKSDB_MAJOR < 8
  typedef std::shared_ptr<rocksdb::Cache> RocksDBCachePtr;
#else
  typedef std::shared_ptr<rocksdb::BlockCache> RocksDBCachePtr;
#endif

  class Cache {
    protected:
      static ErlNifResourceType* m_Cache_RESOURCE;

    public:
      std::mutex mu;

      explicit Cache(RocksDBCachePtr cache);

      ~Cache();

      RocksDBCachePtr cache();

      static void CreateCacheType(ErlNifEnv * Env);
      static void CacheResourceCleanup(ErlNifEnv *Env, void * Arg);

      static Cache * CreateCacheResource(RocksDBCachePtr cache);
      static Cache * RetrieveCacheResource(ErlNifEnv * Env, const ERL_NIF_TERM & CacheTerm);

    private:
      RocksDBCachePtr cache_;
  };

}
