// -------------------------------------------------------------------
// Copyright (c) 2017-2022 Benoit Chesneau. All Rights Reserved.
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
// -------------------------------------------------------------------

#include "env.h"

#include "atoms.h"
#include "util.h"

namespace erocksdb {

ErlNifResourceType * ManagedEnv::m_Env_RESOURCE(NULL);

void
ManagedEnv::CreateEnvType(
    ErlNifEnv * env)
{
    ErlNifResourceFlags flags = (ErlNifResourceFlags)(ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER);
    m_Env_RESOURCE = enif_open_resource_type(env, NULL, "erocksdb_Env",
                                            &ManagedEnv::EnvResourceCleanup,
                                            flags, NULL);
    return;
}   // ManagedEnv::CreateEnvType


void
ManagedEnv::EnvResourceCleanup(
    ErlNifEnv * /*env*/,
    void * arg)
{
    ManagedEnv * env_ptr = (ManagedEnv *)arg;
    env_ptr->~ManagedEnv();
    return;
}

ManagedEnv *
ManagedEnv::CreateEnvResource(rocksdb::Env * env, std::shared_ptr<rocksdb::Env> guard_env)
{
    ManagedEnv * ret_ptr;
    void * alloc_ptr;
    alloc_ptr=enif_alloc_resource(m_Env_RESOURCE, sizeof(ManagedEnv));
    // NOTE
    // ManagedEnv consists of a raw pointer and a shared pointer (which in turn consists of raw pointers)
    // So it has minimal alignment requirements and we are not expected to have alignment issues.
    ret_ptr=new (alloc_ptr) ManagedEnv(env, guard_env);
    return(ret_ptr);
}

ManagedEnv *
ManagedEnv::RetrieveEnvResource(ErlNifEnv * Env, const ERL_NIF_TERM & EnvTerm)
{
    ManagedEnv * ret_ptr;
    if (!enif_get_resource(Env, EnvTerm, m_Env_RESOURCE, (void **)&ret_ptr))
        return NULL;
    return ret_ptr;
}

ManagedEnv::ManagedEnv(rocksdb::Env* env, std::shared_ptr<rocksdb::Env> guard_env) : env_(env), guard_env_(guard_env) {}

ManagedEnv::~ManagedEnv()
{}

const rocksdb::Env* ManagedEnv::env() { return env_; }

ERL_NIF_TERM
NewEnv(
    ErlNifEnv *env,
    int /*argc*/,
    const ERL_NIF_TERM argv[])
{
    ManagedEnv *env_ptr;
    rocksdb::Env *rdb_env;
    if (argv[0] == erocksdb::ATOM_DEFAULT)
    {
        rdb_env = rocksdb::Env::Default();
        env_ptr = ManagedEnv::CreateEnvResource(rdb_env, std::shared_ptr<rocksdb::Env>());
    } else if (argv[0] == erocksdb::ATOM_MEMENV) {
        rdb_env = rocksdb::NewMemEnv(rocksdb::Env::Default());
        // "The caller must delete the result when it is no longer needed."
        // See rocksdb/env.h
        env_ptr = ManagedEnv::CreateEnvResource(rdb_env, std::shared_ptr<rocksdb::Env>(rdb_env));
    } else {
        int arity;
        const ERL_NIF_TERM* tuple;
        if (!enif_get_tuple(env, argv[0], &arity, &tuple)) {
            return enif_make_badarg(env);
        }
        if(arity != 2) {
            return enif_make_badarg(env);
        }
        if(tuple[0] != erocksdb::ATOM_FS_URI) {
            return enif_make_badarg(env);
        }
        std::string fs_uri(4096, '\0');
        int fs_uri_size = enif_get_string(env, tuple[1], &fs_uri[0], fs_uri.size(), ERL_NIF_LATIN1);
        if(fs_uri_size <= 0) {
            return enif_make_badarg(env);
        }
        fs_uri.resize(fs_uri_size - 1);
        std::shared_ptr<rocksdb::Env> env_guard;
        auto config_options = rocksdb::ConfigOptions();
        config_options.ignore_unsupported_options = false;
        rocksdb::Status s = rocksdb::Env::CreateFromUri(config_options, /*env_uri=*/ "", fs_uri, &rdb_env, &env_guard);
        if(!s.ok()) {
            return error_tuple(env, erocksdb::ATOM_ERROR, s);
        }
        env_ptr = ManagedEnv::CreateEnvResource(rdb_env, env_guard);
    }
    // create a resource reference to send erlang
    ERL_NIF_TERM result = enif_make_resource(env, env_ptr);
    // clear the automatic reference from enif_alloc_resource in EnvObject
    enif_release_resource(env_ptr);
    rdb_env = NULL;
    return enif_make_tuple2(env, ATOM_OK, result);
}


ERL_NIF_TERM
SetEnvBackgroundThreads(
        ErlNifEnv* env,
        int argc,
        const ERL_NIF_TERM argv[])
{
    ManagedEnv* env_ptr = ManagedEnv::RetrieveEnvResource(env, argv[0]);

     if(NULL==env_ptr)
        return enif_make_badarg(env);
    rocksdb::Env* rdb_env = (rocksdb::Env* )env_ptr->env();

    int n;
    if(!enif_get_int(env, argv[1], &n))
        return enif_make_badarg(env);

    if(argc==3)
    {
        if(argv[2] == ATOM_PRIORITY_HIGH)
            rdb_env->SetBackgroundThreads(n, rocksdb::Env::Priority::HIGH);
        else if((argv[2] == ATOM_PRIORITY_LOW))
            rdb_env->SetBackgroundThreads(n, rocksdb::Env::Priority::LOW);
        else
            return enif_make_badarg(env);
    }
    else
    {
        rdb_env->SetBackgroundThreads(n);
    }

    return ATOM_OK;
}   // erocksdb::SetBackgroundThreads


ERL_NIF_TERM
DestroyEnv(
        ErlNifEnv* env,
        int /*argc*/,
        const ERL_NIF_TERM argv[])
{

    ManagedEnv* env_ptr = ManagedEnv::RetrieveEnvResource(env, argv[0]);
    if(nullptr==env_ptr)
        return ATOM_OK;

    // What the aim to assign nullptr to a temporary pointer?
    // This funtction does nothing.
    env_ptr = nullptr;

    // At the same time, it shouldn't do anyting:
    // If we call ManagedEnv's destructor, then all the env references across the beam will be invalidated.
    // But when the last reference is destroyed, the env will be destroyed in resource cleanup.
    return ATOM_OK;
}   // erocksdb::DestroyEnv



}

