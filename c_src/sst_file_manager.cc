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

#include "rocksdb/sst_file_manager.h"

#include "atoms.h"
#include "env.h"
#include "sst_file_manager.h"
#include "util.h"

namespace erocksdb {

ErlNifResourceType * SstFileManager::m_SstFileManager_RESOURCE(NULL);

void
SstFileManager::CreateSstFileManagerType( ErlNifEnv * env)
{
    ErlNifResourceFlags flags = (ErlNifResourceFlags)(ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER);
    m_SstFileManager_RESOURCE = enif_open_resource_type(env, NULL, "erocksdb_SstFileManager",
                                            &SstFileManager::SstFileManagerResourceCleanup,
                                            flags, NULL);
    return;
}   // SstFileManager::CreateSstFileManagerType


void
SstFileManager::SstFileManagerResourceCleanup(ErlNifEnv *Env, void * Arg)
{
    SstFileManager* mgr_ptr = (SstFileManager *)Arg;
    mgr_ptr->~SstFileManager();
    mgr_ptr = nullptr;
    return;
}   // SstFileManager::SstFileManagerResourceCleanup


SstFileManager *
SstFileManager::CreateSstFileManagerResource(std::shared_ptr<rocksdb::SstFileManager> mgr)
{
    SstFileManager * ret_ptr;
    void * alloc_ptr;

    alloc_ptr=enif_alloc_resource(m_SstFileManager_RESOURCE, sizeof(SstFileManager));
    ret_ptr=new (alloc_ptr) SstFileManager(mgr);
    return(ret_ptr);
}

SstFileManager *
SstFileManager::RetrieveSstFileManagerResource(ErlNifEnv * Env, const ERL_NIF_TERM & term)
{
    SstFileManager * ret_ptr;
    if (!enif_get_resource(Env, term, m_SstFileManager_RESOURCE, (void **)&ret_ptr))
        return NULL;
    return ret_ptr;
}

SstFileManager::SstFileManager(std::shared_ptr<rocksdb::SstFileManager> Mgr) : mgr_(Mgr) {}

SstFileManager::~SstFileManager()
{
    if(mgr_)
    {
        mgr_ = nullptr;
    }
    return;
}

std::shared_ptr<rocksdb::SstFileManager> SstFileManager::sst_file_manager() {
    auto m = mgr_;
    return m;
}



ERL_NIF_TERM
NewSstFileManager(
        ErlNifEnv* env,
        int argc,
        const ERL_NIF_TERM argv[])
{

    erocksdb::ManagedEnv* env_ptr = erocksdb::ManagedEnv::RetrieveEnvResource(env,argv[0]);
    if(NULL==env_ptr)
        return enif_make_badarg(env);
    
    ErlNifUInt64 rate_bytes_per_sec;
    double max_trash_db_ratio;
    ErlNifUInt64 bytes_max_delete_chunk ;

    if (!enif_get_uint64(env, argv[1], &rate_bytes_per_sec) | 
        !enif_get_double(env, argv[2], &max_trash_db_ratio) | 
        !enif_get_uint64(env, argv[3], &bytes_max_delete_chunk))
    {
        return enif_make_badarg(env);
    }

    rocksdb::Status status; 

    rocksdb::SstFileManager* mgr = 
        rocksdb::NewSstFileManager(
            (rocksdb::Env*)env_ptr->env(),
            nullptr,
            "", /* trash_dir, deprecated */
            rate_bytes_per_sec,
            true, /* delete_existing_trash, deprecate */
            &status,
            max_trash_db_ratio,
            bytes_max_delete_chunk
        );

    if(!status.ok())
        return error_tuple(env, ATOM_ERROR, status);

    std::shared_ptr<rocksdb::SstFileManager> sptr_sst_file_manager(mgr);

    auto mgr_ptr = SstFileManager::CreateSstFileManagerResource(sptr_sst_file_manager);
    // create a resource reference to send erlang
    ERL_NIF_TERM result = enif_make_resource(env, mgr_ptr);
    // clear the automatic reference from enif_alloc_resource in EnvObject
    enif_release_resource(mgr_ptr);
    sptr_sst_file_manager.reset();
    sptr_sst_file_manager = nullptr;
    return enif_make_tuple2(env, ATOM_OK, result);
};

ERL_NIF_TERM
ReleaseSstFileManager(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    SstFileManager* mgr_ptr;
    std::shared_ptr<rocksdb::SstFileManager> mgr;

    mgr_ptr = erocksdb::SstFileManager::RetrieveSstFileManagerResource(env, argv[0]);
    if(nullptr==mgr_ptr)
        return ATOM_OK;
    mgr = mgr_ptr->sst_file_manager();
    mgr.reset();
    mgr = nullptr;
    mgr_ptr = nullptr;
    return ATOM_OK;
}

ERL_NIF_TERM
SstFileManager_Set(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    SstFileManager* mgr_ptr;
    mgr_ptr = erocksdb::SstFileManager::RetrieveSstFileManagerResource(env, argv[0]);
    if(nullptr==mgr_ptr)
        return enif_make_badarg(env);

    std::string cmd;
    if (!enif_get_std_string(env, argv[1], cmd))
        return enif_make_badarg(env);
    
    ErlNifUInt64 ival;
    double dval;
    if(cmd == "MaxAllowedSpaceUsage")
    {
        if(!enif_get_uint64(env, argv[2], &ival))
            return enif_make_badarg(env);

        mgr_ptr->sst_file_manager()->SetMaxAllowedSpaceUsage(ival);
    }
    else if (cmd == "CompactionBufferSize")
    {
        if(!enif_get_uint64(env, argv[2], &ival))
            return enif_make_badarg(env);
        mgr_ptr->sst_file_manager()->SetCompactionBufferSize(ival);
    }
    else if (cmd == "DeleteRateBytesPerSecond")
    {
        if(!enif_get_uint64(env, argv[2], &ival))
            return enif_make_badarg(env);
        mgr_ptr->sst_file_manager()->SetDeleteRateBytesPerSecond(ival);
    }
    else if (cmd == "MaxTrashDBRatio")
    {
        if(!enif_get_double(env, argv[2], &dval))
            return enif_make_badarg(env);
        mgr_ptr->sst_file_manager()->SetMaxTrashDBRatio(dval);
    }
    return ATOM_OK;
}

ERL_NIF_TERM
SstFileManager_Get(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    SstFileManager* mgr_ptr;
    mgr_ptr = erocksdb::SstFileManager::RetrieveSstFileManagerResource(env, argv[0]);
    if(nullptr==mgr_ptr)
        return enif_make_badarg(env);

    std::string cmd;
    if (!enif_get_std_string(env, argv[1], cmd))
        return enif_make_badarg(env);

    if (cmd == "TotalSize")
    {
        return enif_make_uint64(env, mgr_ptr->sst_file_manager()->GetTotalSize());
    }
    else if (cmd == "DeleteRateBytesPerSecond")
    {
        return enif_make_uint64(env, mgr_ptr->sst_file_manager()->GetDeleteRateBytesPerSecond());
    }
    else if (cmd == "MaxTrashDBRatio")
    {
        return enif_make_double(env, mgr_ptr->sst_file_manager()->GetMaxTrashDBRatio());
    }
    else if (cmd == "TotalTrashSize")
    {
        return enif_make_uint64(env, mgr_ptr->sst_file_manager()->GetTotalTrashSize());
    }

    return enif_make_badarg(env);
}

ERL_NIF_TERM
SstFileManager_Is(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    SstFileManager* mgr_ptr;
    mgr_ptr = erocksdb::SstFileManager::RetrieveSstFileManagerResource(env, argv[0]);
    if(nullptr==mgr_ptr)
        return enif_make_badarg(env);

    std::string cmd;
    if (!enif_get_std_string(env, argv[1], cmd))
        return enif_make_badarg(env);

    if (cmd == "MaxAllowedSpaceReached")
    {
        if(mgr_ptr->sst_file_manager()->IsMaxAllowedSpaceReached())
            return ATOM_TRUE;
    }
    else if (cmd == "MaxAllowedSpaceReachedIncludingCompactions")
    {
        if(mgr_ptr->sst_file_manager()->IsMaxAllowedSpaceReachedIncludingCompactions())
            return ATOM_TRUE;
    }
    return ATOM_FALSE;
}

}