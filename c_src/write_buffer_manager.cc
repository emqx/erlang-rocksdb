// Copyright (c) 2018 Benoit Chesneau
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
#include <string>

#include "rocksdb/cache.h"
#include "rocksdb/write_buffer_manager.h"

#include "atoms.h"
#include "env.h"
#include "cache.h"
#include "write_buffer_manager.h"
#include "util.h"

namespace erocksdb {

ErlNifResourceType * WriteBufferManager::m_WriteBufferManager_RESOURCE(NULL);

void
WriteBufferManager::CreateWriteBufferManagerType( ErlNifEnv * env)
{
    ErlNifResourceFlags flags = (ErlNifResourceFlags)(ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER);
    m_WriteBufferManager_RESOURCE = enif_open_resource_type(env, NULL, "erocksdb_WriteBufferManager",
                                            &WriteBufferManager::WriteBufferManagerResourceCleanup,
                                            flags, NULL);
    return;
}   // WriteBufferManager::CreateWriteBufferManagerType


void
WriteBufferManager::WriteBufferManagerResourceCleanup(ErlNifEnv * /*env*/, void * arg)
{
    WriteBufferManager* mgr_ptr = (WriteBufferManager *)arg;
    mgr_ptr->~WriteBufferManager();
    mgr_ptr = nullptr;
    return;
}   // WriteBufferManager::WriteBufferManagerResourceCleanup


WriteBufferManager *
WriteBufferManager::CreateWriteBufferManagerResource(std::shared_ptr<rocksdb::WriteBufferManager> mgr)
{
    WriteBufferManager * ret_ptr;
    void * alloc_ptr;

    alloc_ptr=enif_alloc_resource(m_WriteBufferManager_RESOURCE, sizeof(WriteBufferManager));
    ret_ptr=new (alloc_ptr) WriteBufferManager(mgr);
    return(ret_ptr);
}

WriteBufferManager *
WriteBufferManager::RetrieveWriteBufferManagerResource(ErlNifEnv * Env, const ERL_NIF_TERM & term)
{
    WriteBufferManager * ret_ptr;
    if (!enif_get_resource(Env, term, m_WriteBufferManager_RESOURCE, (void **)&ret_ptr))
        return NULL;
    return ret_ptr;
}

WriteBufferManager::WriteBufferManager(std::shared_ptr<rocksdb::WriteBufferManager> Mgr) : mgr_(Mgr) {}

WriteBufferManager::~WriteBufferManager()
{
    if(mgr_)
    {
        mgr_ = nullptr;
    }
    return;
}

std::shared_ptr<rocksdb::WriteBufferManager> WriteBufferManager::write_buffer_manager() {
    auto m = mgr_;
    return m;
}



ERL_NIF_TERM
NewWriteBufferManager(
        ErlNifEnv* env,
        int argc,
        const ERL_NIF_TERM argv[])
{

    int buffer_size;
    if(!enif_get_int(env, argv[0], &buffer_size))
        return enif_make_badarg(env);

    std::shared_ptr<rocksdb::WriteBufferManager> sptr_write_buffer_manager;
    
    if(argc == 1) {
        sptr_write_buffer_manager = std::make_shared<rocksdb::WriteBufferManager>(buffer_size);
    } else {
        erocksdb::Cache* cache_ptr = erocksdb::Cache::RetrieveCacheResource(env,argv[1]);
        if(NULL==cache_ptr)
            return enif_make_badarg(env);

        sptr_write_buffer_manager = std::make_shared<rocksdb::WriteBufferManager>(buffer_size, cache_ptr->cache());
    }

    auto mgr_ptr = WriteBufferManager::CreateWriteBufferManagerResource(sptr_write_buffer_manager);
    // create a resource reference to send erlang
    ERL_NIF_TERM result = enif_make_resource(env, mgr_ptr);
    // clear the automatic reference from enif_alloc_resource in EnvObject
    enif_release_resource(mgr_ptr);
    sptr_write_buffer_manager.reset();
    sptr_write_buffer_manager = nullptr;
    return enif_make_tuple2(env, ATOM_OK, result);
};

ERL_NIF_TERM
ReleaseWriteBufferManager(ErlNifEnv* env, int /*argc*/, const ERL_NIF_TERM argv[])
{
    WriteBufferManager* mgr_ptr;
    std::shared_ptr<rocksdb::WriteBufferManager> mgr;

    mgr_ptr = erocksdb::WriteBufferManager::RetrieveWriteBufferManagerResource(env, argv[0]);
    if(nullptr==mgr_ptr)
        return ATOM_OK;
    mgr = mgr_ptr->write_buffer_manager();
    mgr.reset();
    mgr = nullptr;
    mgr_ptr = nullptr;
    return ATOM_OK;
}

ERL_NIF_TERM
WriteBufferManager_Get(ErlNifEnv* env, int /*argc*/, const ERL_NIF_TERM argv[])
{
    WriteBufferManager* mgr_ptr;
    mgr_ptr = erocksdb::WriteBufferManager::RetrieveWriteBufferManagerResource(env, argv[0]);
    if(nullptr==mgr_ptr)
        return enif_make_badarg(env);

    std::string cmd;
    if (!enif_get_std_string(env, argv[1], cmd))
        return enif_make_badarg(env);

    if (cmd == "memory_usage")
    {
        return enif_make_uint64(env,mgr_ptr->write_buffer_manager()->memory_usage());
    }
    else if (cmd == "mutable_memtable_memory_usage")
    {
        return enif_make_uint64(env, mgr_ptr->write_buffer_manager()->mutable_memtable_memory_usage());
    } 
    else if (cmd == "buffer_size")
    {
        return enif_make_uint64(env, mgr_ptr->write_buffer_manager()->buffer_size());
    }

    return enif_make_badarg(env);
}

ERL_NIF_TERM
WriteBufferManager_IsEnabled(ErlNifEnv* env, int /*argc*/, const ERL_NIF_TERM argv[])
{
    WriteBufferManager* mgr_ptr;
    mgr_ptr = erocksdb::WriteBufferManager::RetrieveWriteBufferManagerResource(env, argv[0]);
    if(nullptr==mgr_ptr)
        return enif_make_badarg(env);

    if(mgr_ptr->write_buffer_manager()->enabled())
        return ATOM_TRUE;
    return ATOM_FALSE;
}

}