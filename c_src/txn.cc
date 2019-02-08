// -------------------------------------------------------------------
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

#include <iostream>

#include "erl_nif.h"

#include "rocksdb/db.h"
#include "rocksdb/options.h"
#include "rocksdb/slice.h"
#include "rocksdb/utilities/transaction.h"
#include "rocksdb/utilities/optimistic_transaction_db.h"

#include "atoms.h"
#include "refobjects.h"
#include "util.h"

#include "erocksdb_db.h"

struct Txn
{
    rocksdb::Transaction *txn;
    rocksdb::DB *base_db;
    ErlNifEnv *env;
};

static void cleanup_txn(Txn* txn)
{
    if(txn->env != nullptr) {
        enif_free_env(txn->env);
        txn->env = nullptr;
    }
    txn->base_db = nullptr;
    delete txn->txn;
}

namespace erocksdb {

    ErlNifResourceType *m_Txn_RESOURCE;

    void
    txn_resource_cleanup(ErlNifEnv * /*env*/, void *arg)
    {
        Txn* txn = reinterpret_cast<Txn*>(arg);
        cleanup_txn(txn);
    }

    void
    CreateTxnType(ErlNifEnv *env)
    {
        ErlNifResourceFlags flags = (ErlNifResourceFlags)(ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER);
        m_Txn_RESOURCE = enif_open_resource_type(env, NULL, "rocksdb_Transaction",
                                                 txn_resource_cleanup, flags, NULL);
        return;
    }

    ERL_NIF_TERM
    NewTxn(ErlNifEnv* env,
           int argc,
           const ERL_NIF_TERM argv[])
    {
        if(argc != 2) {
            return enif_make_badarg(env);
        }
        Txn* txn = reinterpret_cast<Txn*>(enif_alloc_resource(m_Txn_RESOURCE, sizeof(Txn)));

        // not sure that we need this, since there are multiple usable arities
        rocksdb::OptimisticTransactionOptions txn_options;
        rocksdb::WriteOptions write_options;
        fold(env, argv[1], parse_write_option, write_options);
        ReferencePtr<DbObject> db_ptr;
        if(!enif_get_db(env, argv[0], &db_ptr)) {
            return enif_make_badarg(env);
        }

        rocksdb::OptimisticTransactionDB* db =
            reinterpret_cast<rocksdb::OptimisticTransactionDB*>(db_ptr->m_Db);
        // txn_options.set_snapshot = true;
        txn->txn = db->BeginTransaction(write_options, txn_options);
        txn->base_db = db->GetBaseDB();
        txn->env = enif_alloc_env();

        ERL_NIF_TERM result = enif_make_resource(env, txn);
        enif_release_resource(txn);
        return enif_make_tuple2(env, ATOM_OK, result);
    }

    ERL_NIF_TERM
    PutTxn(ErlNifEnv* env,
           int argc,
           const ERL_NIF_TERM argv[])
    {
        ReferencePtr<erocksdb::ColumnFamilyObject> cf_ptr;

        ErlNifBinary key, value;
        rocksdb::Transaction *t = nullptr;
        Txn *txn = nullptr;

        if(!enif_get_resource(env, argv[0], m_Txn_RESOURCE, (void **) &txn)) {
            return enif_make_badarg(env);
        }
        t = txn->txn;
        if(t == nullptr ) {
            return enif_make_badarg(env);
        }

        if( argc == 3 ) { // default
            if(!enif_inspect_binary(env, argv[1], &key) ||
               !enif_inspect_binary(env, argv[2], &value)) {
                return enif_make_badarg(env);
            }

            enif_make_copy(txn->env, argv[1]);
            enif_make_copy(txn->env, argv[2]);

            rocksdb::Slice key_slice(reinterpret_cast<char*>(key.data), key.size);
            rocksdb::Slice value_slice(reinterpret_cast<char*>(value.data), value.size);
            t->Put(key_slice, value_slice);

        } else if( argc == 4 ) { // cf
            if(!enif_get_cf(env, argv[1], &cf_ptr) ||
               !enif_inspect_binary(env, argv[2], &key) ||
               !enif_inspect_binary(env, argv[3], &value)) {
                return enif_make_badarg(env);
            }

            enif_make_copy(txn->env, argv[1]);
            enif_make_copy(txn->env, argv[2]);
            enif_make_copy(txn->env, argv[3]);

            rocksdb::Slice key_slice(reinterpret_cast<char*>(key.data), key.size);
            rocksdb::Slice value_slice(reinterpret_cast<char*>(value.data), value.size);
            erocksdb::ColumnFamilyObject* cf = cf_ptr.get();
            t->Put(cf->m_ColumnFamily, key_slice, value_slice);

        } else {
            // not sure this is possible with modern nifs, but
            return enif_make_badarg(env);
        }
        return ATOM_OK;
    }

    ERL_NIF_TERM
    GetTxn(ErlNifEnv* env,
           int argc,
           const ERL_NIF_TERM argv[])
    {
        ReferencePtr<erocksdb::ColumnFamilyObject> cf_ptr;
        rocksdb::Transaction *t = nullptr;
        Txn *txn = nullptr;

        if(!enif_get_resource(env, argv[0], m_Txn_RESOURCE, (void **) &txn)) {
            return enif_make_badarg(env);
        }
        t = txn->txn;
        if(t == nullptr ) {
            return enif_make_badarg(env);
        }

        int i = 1;
        if(argc == 4) {
            i = 2;
        }

        rocksdb::Slice key;
        if(!binary_to_slice(env, argv[i], &key) || t == nullptr ) {
            return enif_make_badarg(env);
        }

        rocksdb::ReadOptions opts;
        fold(env, argv[i+1], parse_read_option, opts);

        rocksdb::Status status;
        rocksdb::PinnableSlice pvalue;
        if(argc == 4) {
            ReferencePtr<ColumnFamilyObject> cf_ptr;
            if(!enif_get_cf(env, argv[1], &cf_ptr)) {
                return enif_make_badarg(env);
            }
            status = t->GetForUpdate(opts, cf_ptr->m_ColumnFamily, key, &pvalue);
        } else {
            status = t->GetForUpdate(opts, txn->base_db->DefaultColumnFamily(), key, &pvalue);
        }

        if (!status.ok())
        {

            if (status.IsNotFound())
                return ATOM_NOT_FOUND;

            if (status.IsCorruption())
                return error_tuple(env, ATOM_CORRUPTION, status);

            return error_tuple(env, ATOM_UNKNOWN_STATUS_ERROR, status);
        }

        ERL_NIF_TERM value_bin;
        memcpy(enif_make_new_binary(env, pvalue.size(), &value_bin), pvalue.data(), pvalue.size());
        pvalue.Reset();
        return enif_make_tuple2(env, ATOM_OK, value_bin);
    }

    ERL_NIF_TERM
    MergeTxn(ErlNifEnv* env,
             int argc,
             const ERL_NIF_TERM argv[])
    {
        ReferencePtr<erocksdb::ColumnFamilyObject> cf_ptr;
        rocksdb::Transaction *t = nullptr;
        Txn *txn = nullptr;

        if(!enif_get_resource(env, argv[0], m_Txn_RESOURCE, (void **) &txn)) {
            return enif_make_badarg(env);
        }
        t = txn->txn;
        if(t == nullptr ) {
            return enif_make_badarg(env);
        }

        ErlNifBinary key, value;

        rocksdb::Status status;
        rocksdb::ColumnFamilyHandle *cfh;
        if (argc == 4) {
            if(!enif_get_cf(env, argv[1], &cf_ptr) ||
               !enif_inspect_binary(env, argv[2], &key) ||
               !enif_inspect_binary(env, argv[3], &value)) {
                return enif_make_badarg(env);
            }
            cfh = cf_ptr->m_ColumnFamily;
        } else if (argc == 3) {
            if(!enif_inspect_binary(env, argv[1], &key) ||
               !enif_inspect_binary(env, argv[2], &value)) {
                return enif_make_badarg(env);
            }
            cfh = txn->base_db->DefaultColumnFamily();
        } else {
            return enif_make_badarg(env);
        }

        rocksdb::Slice key_slice(reinterpret_cast<char*>(key.data), key.size);
        rocksdb::Slice value_slice(reinterpret_cast<char*>(value.data), value.size);
        status = t->Merge(cfh, key_slice, value_slice);

        if(status.ok()) {
            return ATOM_OK;
        } else {
            return error_tuple(env, ATOM_ERROR, status);
        }
    }

    ERL_NIF_TERM
    DelTxn(ErlNifEnv* env,
           int argc,
           const ERL_NIF_TERM argv[])
    {
        ReferencePtr<erocksdb::ColumnFamilyObject> cf_ptr;
        rocksdb::Transaction *t = nullptr;
        Txn *txn = nullptr;

        if(!enif_get_resource(env, argv[0], m_Txn_RESOURCE, (void **) &txn)) {
            return enif_make_badarg(env);
        }
        t = txn->txn;
        if(t == nullptr ) {
            return enif_make_badarg(env);
        }

        ErlNifBinary key;
        rocksdb::Status status;
        rocksdb::ColumnFamilyHandle *cfh;
        if (argc > 2)
        {
            if(!enif_get_cf(env, argv[1], &cf_ptr) ||
               !enif_inspect_binary(env, argv[2], &key))
                return enif_make_badarg(env);
            cfh = cf_ptr->m_ColumnFamily;
        }
        else
        {
            if(!enif_inspect_binary(env, argv[1], &key))
                return enif_make_badarg(env);
            cfh = txn->base_db->DefaultColumnFamily();
        }
        rocksdb::Slice key_slice(reinterpret_cast<char*>(key.data), key.size);
        status = t->Delete(cfh, key_slice);

        if(status.ok()) {
            return ATOM_OK;
        } else {
            return error_tuple(env, ATOM_ERROR, status);
        }
    }

    ERL_NIF_TERM
    CommitTxn(ErlNifEnv* env,
              int /*argc*/,
              const ERL_NIF_TERM argv[])
    {

        ReferencePtr<erocksdb::ColumnFamilyObject> cf_ptr;
        rocksdb::Transaction *t = nullptr;
        Txn *txn = nullptr;

        if(!enif_get_resource(env, argv[0], m_Txn_RESOURCE, (void **) &txn)) {
            return enif_make_badarg(env);
        }
        t = txn->txn;
        if(t == nullptr ) {
            return enif_make_badarg(env);
        }

        rocksdb::Status s = t->Commit();
        delete t;
        txn->txn = nullptr;

        if( s.ok() ) {
            return ATOM_OK;
        } else {
            return error_tuple(env, ATOM_ERROR, s);
        }
    }


}
