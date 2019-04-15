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

#include <string>

#include "env.h"

#include "atoms.h"
#include "util.h"

#include "rocksdb/cloud/cloud_env_options.h"


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
    void * /*arg*/)
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
    } else if (argv[0] == erocksdb::ATOM_MEMENV) {
        rdb_env = rocksdb::NewMemEnv(rocksdb::Env::Default());
    } else {
        return enif_make_badarg(env);
    }
    env_ptr = ManagedEnv::CreateEnvResource(rdb_env);
    // create a resource reference to send erlang
    ERL_NIF_TERM result = enif_make_resource(env, env_ptr);
    // clear the automatic reference from enif_alloc_resource in EnvObject
    enif_release_resource(env_ptr);
    rdb_env = NULL;
    return enif_make_tuple2(env, ATOM_OK, result);
}

ERL_NIF_TERM
parse_aws_access_credentials(
        ErlNifEnv* env,
        ERL_NIF_TERM item,
        rocksdb::AwsCloudAccessCredentials& credentials)
{
    int arity;
    const ERL_NIF_TERM* option;

    if (enif_get_tuple(env, item, &arity, &option) && 2==arity)
    {
        if(option[0] == erocksdb::ATOM_ACCESS_KEY_ID) {
            std::string access_key_id;
            if (enif_get_std_string(env, option[1], access_key_id))
                credentials.access_key_id = access_key_id;
        } else if(option[0] == erocksdb::ATOM_SECRET_KEY) {
            std::string secret_key;
            if (enif_get_std_string(env, option[1], secret_key))
                credentials.secret_key = secret_key;
        }
    }
    return erocksdb::ATOM_OK;
}


ERL_NIF_TERM
parse_aws_options(ErlNifEnv* env, ERL_NIF_TERM item, rocksdb::AwsOptions& opts) {
    int arity;
    const ERL_NIF_TERM* option;

    if (enif_get_tuple(env, item, &arity, &option) && 2==arity)
    {
        if(option[0] == erocksdb::ATOM_REQUEST_TIMEOUT_MS) {
            long int req_timeout;
            if(enif_get_long(env, option[1], &req_timeout))
                opts.requestTimeoutMs = reinterpret_cast<long>(req_timeout);
        } else if(option[0] == erocksdb::ATOM_CONNECT_TIMEOUT_MS) {
            long int connect_timeout;
            if(enif_get_long(env, option[1], &connect_timeout))
                opts.requestTimeoutMs = reinterpret_cast<long>(connect_timeout);
        } else if(option[0] == erocksdb::ATOM_ENDPOINT_OVERRIDE) {
            std::string endpointOverride;
            if (enif_get_std_string(env, option[1], endpointOverride))
                opts.endpointOverride = endpointOverride;
        } else if(option[0] == erocksdb::ATOM_SCHEME) {
            std::string scheme;
            if (enif_get_std_string(env, option[1], scheme))
                opts.scheme = scheme;
        } else if(option[0] == erocksdb::ATOM_VERIFY_SSL) {
            if (option[1] == ATOM_TRUE) {
                opts.verifySSL = 1;
            } else {
                opts.verifySSL = 0;
            }
        } else if(option[0] == erocksdb::ATOM_PROXY_HOST) {
            std::string proxyHost;
            if (enif_get_std_string(env, option[1], proxyHost))
                opts.proxyHost = proxyHost;
        } else if(option[0] == erocksdb::ATOM_PROXY_SCHEME) {
            std::string proxyScheme;
            if (enif_get_std_string(env, option[1], proxyScheme))
                opts.proxyScheme = proxyScheme;
        } else if(option[0] == erocksdb::ATOM_PROXY_PORT) {
            unsigned int proxyPort;
            if (enif_get_uint(env, option[1], &proxyPort))
                opts.proxyPort = proxyPort;
         }else if(option[0] == erocksdb::ATOM_PROXY_USER_NAME) {
            std::string proxyUserName;
            if (enif_get_std_string(env, option[1], proxyUserName))
                opts.proxyUserName = proxyUserName;
        } else if(option[0] == erocksdb::ATOM_PROXY_PASSWORD) {
            std::string proxyPassword;
            if (enif_get_std_string(env, option[1], proxyPassword))
                opts.proxyScheme = proxyPassword;
        }
    }
    return erocksdb::ATOM_OK;
}

ERL_NIF_TERM
parse_cloud_env_options(ErlNifEnv* env, ERL_NIF_TERM item, rocksdb::CloudEnvOptions& opts) {
    int arity;
    const ERL_NIF_TERM* option;

    if (enif_get_tuple(env, item, &arity, &option) && 2==arity)
    {
        if(option[0] == erocksdb::ATOM_CREDENTIALS) {
            rocksdb::AwsCloudAccessCredentials credentials;
            fold(env, option[1], parse_aws_access_credentials, credentials);
            opts.credentials = credentials;
        } else if(option[0] == erocksdb::ATOM_AWS_OPTIONS) {
            rocksdb::AwsOptions aws_options;
            fold(env, option[1], parse_aws_options, aws_options);
            opts.aws_options = aws_options;
        } else if (option[0] == erocksdb::ATOM_KEEP_LOCAL_SST_FILES) {
            opts.keep_local_sst_files = (option[1] == erocksdb::ATOM_TRUE);
        }  else if (option[0] == erocksdb::ATOM_KEEP_LOCAL_LOG_FILES) {
            opts.keep_local_log_files = (option[1] == erocksdb::ATOM_TRUE);
        } else if (option[0] == erocksdb::ATOM_PURGER_PERIODICITY_MILLIS) {
            ErlNifUInt64 purger_periodicity_millis;
            if (enif_get_uint64(env, option[1], &purger_periodicity_millis))
                 opts.purger_periodicity_millis = static_cast<uint64_t>(purger_periodicity_millis);
        }  else if (option[0] == erocksdb::ATOM_VALIDATE_FILESIZE) {
            opts.validate_filesize = (option[1] == erocksdb::ATOM_TRUE);
        } else if (option[0] == erocksdb::ATOM_SERVER_SIDE_ENCRYPTION) {
            opts.server_side_encryption = (option[1] == erocksdb::ATOM_TRUE);
        } else if (option[0] == erocksdb::ATOM_ENCRYPTION_KEY_ID) {
            std::string encryption_key_id;
            if (enif_get_std_string(env, option[1], encryption_key_id))
                opts.encryption_key_id = encryption_key_id;
        } else if (option[0] == erocksdb::ATOM_CREATE_BUCKET_IF_MISSING) {
            opts.create_bucket_if_missing = (option[1] == erocksdb::ATOM_TRUE);
        } else if(option[0] == erocksdb::ATOM_REQUEST_TIMEOUT_MS) {
            ErlNifUInt64 req_timeout;
            if(enif_get_uint64(env, option[1], &req_timeout))
                opts.request_timeout_ms = static_cast<uint64_t>(req_timeout);
        } else if (option[0] == erocksdb::ATOM_RUN_PURGER) {
            opts.run_purger = (option[1] == erocksdb::ATOM_TRUE);
        } else if (option[0] == erocksdb::ATOM_EPHEMERAL_RESYNC_ON_OPEN) {
            opts.ephemeral_resync_on_open = (option[1] == erocksdb::ATOM_TRUE);
        } else if (option[0] == erocksdb::ATOM_SKIP_DBID_VERIFICATION)
            opts.skip_dbid_verification = (option[1] == erocksdb::ATOM_TRUE);
    }
    return erocksdb::ATOM_OK;
}

ERL_NIF_TERM
NewCloudEnv(
    ErlNifEnv *env,
    int /*argc*/,
    const ERL_NIF_TERM argv[])
{

    ManagedEnv *env_ptr;
    rocksdb::CloudEnv *cloud_env;

    std::string src_bucket_name;
    std::string src_object_prefix;
    std::string src_bucket_region;
    std::string dest_bucket_name;
    std::string dest_object_prefix;
    std::string dest_bucket_region;


    if(!enif_get_std_string(env, argv[0], src_bucket_name) ||
            !enif_get_std_string(env, argv[1], src_object_prefix) ||
            !enif_get_std_string(env, argv[2], src_bucket_region) ||
            !enif_get_std_string(env, argv[3], dest_bucket_name) ||
            !enif_get_std_string(env, argv[4], dest_object_prefix) ||
            !enif_get_std_string(env, argv[5], dest_bucket_region))
        return enif_make_badarg(env);

    rocksdb::CloudEnvOptions opts;
    fold(env, argv[6], parse_cloud_env_options, opts);

    rocksdb::Status status = rocksdb::CloudEnv::NewAwsEnv(rocksdb::Env::Default(),
            reinterpret_cast<const std::string&>(src_bucket_name),
            reinterpret_cast<const std::string&>(src_object_prefix),
            reinterpret_cast<const std::string&>(src_bucket_region),
            reinterpret_cast<const std::string&>(dest_bucket_name),
            reinterpret_cast<const std::string&>(dest_object_prefix),
            reinterpret_cast<const std::string&>(dest_bucket_region),
            opts,
            nullptr,
            &cloud_env);

    if(!status.ok())
        return error_tuple(env, ATOM_ERROR_DB_OPEN, status);

    env_ptr = ManagedEnv::CreateEnvResource(cloud_env);

    // create a resource reference to send erlang
    ERL_NIF_TERM result = enif_make_resource(env, env_ptr);
    // clear the automatic reference from enif_alloc_resource in EnvObject
    enif_release_resource(env_ptr);
    cloud_env = NULL;
    return enif_make_tuple2(env, ATOM_OK, result);
}

ERL_NIF_TERM
CloudEnvEmptyBucket(
        ErlNifEnv* env,
        int /* argc */,
        const ERL_NIF_TERM argv[])
{

    ManagedEnv* env_ptr = ManagedEnv::RetrieveEnvResource(env, argv[0]);

    if(NULL==env_ptr)
        return enif_make_badarg(env);

    rocksdb::CloudEnv* cloud_env = (rocksdb::CloudEnv* )env_ptr->env();

    std::string bucket_prefix;
    std::string path_prefix;
    if(!enif_get_std_string(env, argv[1], bucket_prefix) ||
            !enif_get_std_string(env, argv[2], path_prefix))
        return enif_make_badarg(env);

    try {
        cloud_env->EmptyBucket(bucket_prefix, path_prefix);
    } catch (const std::exception& e) {
        // pass through
        return ATOM_ERROR;
    }
    return ATOM_OK;
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

    env_ptr = nullptr;
    return ATOM_OK;
}   // erocksdb::DestroyEnv



}

