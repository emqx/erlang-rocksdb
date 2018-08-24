// -------------------------------------------------------------------
// Copyright (c) 2011-2013 Basho Technologies, Inc. All Rights Reserved.
// Copyright (c) 2016-2017 Benoit Chesneau. All Rights Reserved.
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

#pragma once
#ifndef INCL_UTIL_H
#define INCL_UTIL_H

#include "erl_nif.h"

// Forward declaration
namespace rocksdb {
    class Status;
    class Slice;
}

// Forward declaration
namespace erocksdb {
    template <class TargetT>
    class ReferencePtr;
    class DbObject;
    class ColumnFamilyObject;
    class BackupEngineObject;
}

ERL_NIF_TERM error_einval(ErlNifEnv* env);
ERL_NIF_TERM error_tuple(ErlNifEnv* env, ERL_NIF_TERM error, rocksdb::Status& status);
ERL_NIF_TERM slice_to_binary(ErlNifEnv* env, rocksdb::Slice s);

int binary_to_slice(ErlNifEnv* env, ERL_NIF_TERM val, rocksdb::Slice *slice);

int enif_get_db(ErlNifEnv* env, ERL_NIF_TERM dbval, erocksdb::ReferencePtr<erocksdb::DbObject>* db_ptr);
int enif_get_cf(ErlNifEnv* env, ERL_NIF_TERM dbval, erocksdb::ReferencePtr<erocksdb::ColumnFamilyObject>* cf_ptr);
int enif_get_backup_engine(ErlNifEnv* env, ERL_NIF_TERM bal, erocksdb::ReferencePtr<erocksdb::BackupEngineObject>* backup_engine_ptr);


int parse_int(const std::string& value);

// Cleanup function for C++ object created with enif allocator via C++
// placement syntax which necessitates explicit invocation of the object's
// destructor.
template <typename T>
void cleanup_obj_ptr(T*& ptr)
{
    if (ptr != nullptr) {
        ptr->~T();
        enif_free(ptr);
        ptr = nullptr;
    }
}

#endif
