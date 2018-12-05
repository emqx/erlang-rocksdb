// Copyright (c) 2016-2017 Benoit Chesneau
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
#include "rocksdb/cache.h"
#include "cache.h"
#include "util.h"

namespace erocksdb {

ErlNifResourceType * Cache::m_Cache_RESOURCE(NULL);

void
Cache::CreateCacheType(ErlNifEnv * env)
{
    ErlNifResourceFlags flags = (ErlNifResourceFlags)(ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER);
    m_Cache_RESOURCE = enif_open_resource_type(env, NULL, "erocksdb_Cache",
                                            &Cache::CacheResourceCleanup,
                                            flags, NULL);
    return;
}   // Cache::CreateCacheType


void
Cache::CacheResourceCleanup(ErlNifEnv * /*env*/, void * arg)
{
    Cache* cache_ptr = (Cache *)arg;
    cache_ptr->~Cache();
    cache_ptr = nullptr;
    return;
}   // Cache::CacheResourceCleanup


Cache *
Cache::CreateCacheResource(std::shared_ptr<rocksdb::Cache> cache)
{
    Cache * ret_ptr;
    void * alloc_ptr;

    alloc_ptr=enif_alloc_resource(m_Cache_RESOURCE, sizeof(Cache));
    ret_ptr=new (alloc_ptr) Cache(cache);
    return(ret_ptr);
}

Cache *
Cache::RetrieveCacheResource(ErlNifEnv * Env, const ERL_NIF_TERM & CacheTerm)
{
    Cache * ret_ptr;
    if (!enif_get_resource(Env, CacheTerm, m_Cache_RESOURCE, (void **)&ret_ptr))
        return NULL;
    return ret_ptr;
}

Cache::Cache(std::shared_ptr<rocksdb::Cache> Cache) : cache_(Cache) {}

Cache::~Cache()
{
    if(cache_)
    {
        cache_ = nullptr;
    }
    return;
}

std::shared_ptr<rocksdb::Cache> Cache::cache() {
    auto c = cache_;
    return c;
}


ERL_NIF_TERM
NewLRUCache(
        ErlNifEnv* env,
        int /*argc*/,
        const ERL_NIF_TERM argv[])
{
    ErlNifUInt64 capacity;
    Cache* cache_ptr;
    if(!enif_get_uint64(env, argv[0], &capacity))
        return enif_make_badarg(env);

    std::shared_ptr<rocksdb::Cache> cache = rocksdb::NewLRUCache(capacity);
    cache_ptr = Cache::CreateCacheResource(cache);
    // create a resource reference to send erlang
    ERL_NIF_TERM result = enif_make_resource(env, cache_ptr);
    // clear the automatic reference from enif_alloc_resource in EnvObject
    enif_release_resource(cache_ptr);
    cache.reset();
    cache = nullptr;
    return enif_make_tuple2(env, ATOM_OK, result);
}

ERL_NIF_TERM
NewClockCache(
        ErlNifEnv* env,
        int /*argc*/,
        const ERL_NIF_TERM argv[])
{
    ErlNifUInt64 capacity;
    Cache* cache_ptr;
    if(!enif_get_uint64(env, argv[0], &capacity))
        return enif_make_badarg(env);
    std::shared_ptr<rocksdb::Cache> cache = rocksdb::NewClockCache(capacity);
    cache_ptr = Cache::CreateCacheResource(cache);
    // create a resource reference to send erlang
    ERL_NIF_TERM result = enif_make_resource(env, cache_ptr);
    // clear the automatic reference from enif_alloc_resource in EnvObject
    enif_release_resource(cache_ptr);
    cache.reset();
    cache = nullptr;
    return enif_make_tuple2(env, ATOM_OK, result);
}
ERL_NIF_TERM
GetCapacity(ErlNifEnv* env, int /*argc*/, const ERL_NIF_TERM argv[])
{

    Cache* cache_ptr;
    std::shared_ptr<rocksdb::Cache> cache;

    cache_ptr = erocksdb::Cache::RetrieveCacheResource(env, argv[0]);
    if(nullptr==cache_ptr)
        return enif_make_badarg(env);

    std::lock_guard<std::mutex> guard(cache_ptr->mu);
    cache = cache_ptr->cache();
    ERL_NIF_TERM usage = enif_make_uint64(env,cache->GetCapacity());
    return usage;
}

ERL_NIF_TERM
SetCapacity(ErlNifEnv* env, int /*argc*/, const ERL_NIF_TERM argv[])
{

    Cache* cache_ptr;
    std::shared_ptr<rocksdb::Cache> cache;
    ErlNifUInt64 capacity;

    cache_ptr = erocksdb::Cache::RetrieveCacheResource(env, argv[0]);
    if(nullptr==cache_ptr)
        return enif_make_badarg(env);

    if(!enif_get_uint64(env, argv[1], &capacity))
        return enif_make_badarg(env);

    std::lock_guard<std::mutex> guard(cache_ptr->mu);
    cache = cache_ptr->cache();
    cache->SetCapacity(capacity);
    return ATOM_OK;
}

ERL_NIF_TERM
SetStrictCapacityLimit(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{

    Cache* cache_ptr;
    std::shared_ptr<rocksdb::Cache> cache;
    bool strict_capacity_limit;

    if(argc < 2)
        return enif_make_badarg(env);

    cache_ptr = erocksdb::Cache::RetrieveCacheResource(env, argv[0]);
    if(nullptr==cache_ptr)
        return enif_make_badarg(env);

    strict_capacity_limit = (argv[1] == ATOM_TRUE);
    std::lock_guard<std::mutex> guard(cache_ptr->mu);
    cache = cache_ptr->cache();
    cache->SetStrictCapacityLimit(strict_capacity_limit);
    return ATOM_OK;
}

ERL_NIF_TERM
GetUsage(ErlNifEnv* env, int /*argc*/, const ERL_NIF_TERM argv[])
{

    Cache* cache_ptr;
    std::shared_ptr<rocksdb::Cache> cache;

    cache_ptr = erocksdb::Cache::RetrieveCacheResource(env, argv[0]);
    if(nullptr==cache_ptr)
        return enif_make_badarg(env);

    cache = cache_ptr->cache();
    ERL_NIF_TERM usage = enif_make_uint64(env,cache->GetUsage());
    return usage;
}

ERL_NIF_TERM
GetPinnedUsage(ErlNifEnv* env, int /*argc*/, const ERL_NIF_TERM argv[])
{
    Cache* cache_ptr;
    std::shared_ptr<rocksdb::Cache> cache;

    cache_ptr = erocksdb::Cache::RetrieveCacheResource(env, argv[0]);
    if(nullptr==cache_ptr)
        return enif_make_badarg(env);

    cache = cache_ptr->cache();
    ERL_NIF_TERM usage = enif_make_uint64(env,cache->GetPinnedUsage());
    return usage;
}


ERL_NIF_TERM
ReleaseCache(ErlNifEnv* env, int /*argc*/, const ERL_NIF_TERM argv[])
{
    Cache* cache_ptr;
    std::shared_ptr<rocksdb::Cache> cache;

    cache_ptr = erocksdb::Cache::RetrieveCacheResource(env, argv[0]);
    if(nullptr==cache_ptr)
        return ATOM_OK;
    cache = cache_ptr->cache();
    cache.reset();
    cache = nullptr;
    cache_ptr = nullptr;
    return ATOM_OK;
}


}
