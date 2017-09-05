// -------------------------------------------------------------------
// Copyright (c) 2017 Benoit Chesneau. All Rights Reserved.
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


#include <vector>

#include "erocksdb.h"

#include "rocksdb/db.h"
#include "rocksdb/env.h"
#ifndef INCL_REFOBJECTS_H
    #include "refobjects.h"
#endif

#ifndef ATOMS_H
    #include "atoms.h"
#endif

#ifndef INCL_UTIL_H
    #include "util.h"
#endif

#include "env.h"

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
    ErlNifEnv * Env,
    void * Arg)
{
    return;
}

ManagedEnv *
ManagedEnv::CreateEnvResource(rocksdb::Env * env)
{
    ManagedEnv * ret_ptr;
    void * alloc_ptr;

    alloc_ptr=enif_alloc_resource(m_Env_RESOURCE, sizeof(ManagedEnv));
    ret_ptr=new (alloc_ptr) ManagedEnv(env);
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

ManagedEnv::ManagedEnv(rocksdb::Env * Env) : env_(Env) {}

ManagedEnv::~ManagedEnv()
{
    if(env_)
    {
        delete env_;
        env_ = NULL;
    }

    return;
}

const rocksdb::Env* ManagedEnv::env() { return env_; }


ERL_NIF_TERM
DefaultEnv(
    ErlNifEnv* env,
    int argc,
    const ERL_NIF_TERM argv[])
{
    ManagedEnv* env_ptr;
    rocksdb::Env* default_env = rocksdb::Env::Default();

    env_ptr = ManagedEnv::CreateEnvResource(default_env);

    // create a resource reference to send erlang
    ERL_NIF_TERM result = enif_make_resource(env, env_ptr);
    // clear the automatic reference from enif_alloc_resource in EnvObject
    enif_release_resource(env_ptr);
    default_env = NULL;

    return enif_make_tuple2(env, ATOM_OK, result);
}   // erocksdb::SnapShot


ERL_NIF_TERM
MemEnv(
    ErlNifEnv* env,
    int argc,
    const ERL_NIF_TERM argv[])
{
    ManagedEnv* env_ptr;
    rocksdb::Env* mem_env = rocksdb::NewMemEnv(rocksdb::Env::Default());

    env_ptr = ManagedEnv::CreateEnvResource(mem_env);

    // create a resource reference to send erlang
    ERL_NIF_TERM result = enif_make_resource(env, env_ptr);
    // clear the automatic reference from enif_alloc_resource in EnvObject
    enif_release_resource(env_ptr);
    mem_env = NULL;

    return enif_make_tuple2(env, ATOM_OK, result);
}   // erocksdb::SnapShot


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
        int argc,
        const ERL_NIF_TERM argv[])
{

    ManagedEnv* env_ptr = ManagedEnv::RetrieveEnvResource(env, argv[0]);
    if(NULL==env_ptr)
        return ATOM_OK;

    delete env_ptr;
    return ATOM_OK;
}   // erocksdb::DestroyEnv



}

