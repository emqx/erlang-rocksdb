#include <vector>

#include "erocksdb.h"

#include "rocksdb/db.h"
#include "rocksdb/cache.h"


namespace erocksdb {

  class Cache {
    protected:
      static ErlNifResourceType* m_Cache_RESOURCE;

    public:
      explicit Cache(std::shared_ptr<rocksdb::Cache> cache);

      ~Cache();

      std::shared_ptr<rocksdb::Cache> cache();

      static void CreateCacheType(ErlNifEnv * Env);
      static void CacheResourceCleanup(ErlNifEnv *Env, void * Arg);

      static Cache * CreateCacheResource(std::shared_ptr<rocksdb::Cache> cache);
      static Cache * RetrieveCacheResource(ErlNifEnv * Env, const ERL_NIF_TERM & CacheTerm);
    
    private:
      std::shared_ptr<rocksdb::Cache> cache_;
  };
  
}