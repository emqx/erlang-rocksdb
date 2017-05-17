#include <vector>

#include "erocksdb.h"

#include "rocksdb/db.h"
#include "rocksdb/env.h"


namespace erocksdb {

  class ManagedEnv {
    protected:
      static ErlNifResourceType* m_Env_RESOURCE;

    public:
      explicit ManagedEnv(rocksdb::Env * Env);

      ~ManagedEnv();

      const rocksdb::Env* env();

      static void CreateEnvType(ErlNifEnv * Env);
      static void EnvResourceCleanup(ErlNifEnv *Env, void * Arg);

      static ManagedEnv * CreateEnvResource(rocksdb::Env * env);
      static ManagedEnv * RetrieveEnvResource(ErlNifEnv * Env, const ERL_NIF_TERM & EnvTerm);
    
    private:
      const rocksdb::Env* env_;
  };
  
}