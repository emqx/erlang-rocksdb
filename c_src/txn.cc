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
#include "transaction_log.h"

struct Txn
{
    rocksdb::Transaction * txn;
    ErlNifEnv* env;
};

struct OptTxnDb
{
    rocksdb::OptimisticTransactionDB* db;
    ErlNifEnv* env;
};

static void cleanup_txn(Txn* txn)
{
    if(txn->env != nullptr) {
        enif_free_env(txn->env);
        txn->env = nullptr;
    }
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

        // {"txn", 3, erocksdb::NewTxn, ERL_NIF_REGULAR_BOUND},
        // {"txn_put", 3, erocksdb::PutTxn, ERL_NIF_REGULAR_BOUND},
        // {"txn_get", 3, erocksdb::GetTxn, ERL_NIF_REGULAR_BOUND},
        // {"txn_merge", 3, erocksdb::MergeTxn, ERL_NIF_REGULAR_BOUND},
        // {"txn_delete", 3, erocksdb::DelTxn, ERL_NIF_REGULAR_BOUND},
        // {"txn_commit", 3, erocksdb::CommitTxn, ERL_NIF_DIRTY_JOB_IO_BOUND},   

    ERL_NIF_TERM
    NewTxn(ErlNifEnv* env,
           int argc,
           const ERL_NIF_TERM argv[])
    {
        if(argc != 2) {
            return enif_make_badarg(env);
        }
        // rocksdb::Transaction* t = reinterpret_cast<rocksdb::Transaction*>(enif_alloc(sizeof(rocksdb::Transaction)));
        Txn* txn = reinterpret_cast<Txn*>(enif_alloc_resource(m_Txn_RESOURCE, sizeof(Txn)));

        // not sure that we need this, since there are multiple usable arities
        // rocksdb::OptimisticTransactionOptions txn_options txn_options;
        rocksdb::WriteOptions write_options;
        fold(env, argv[1], parse_write_option, write_options);
        ReferencePtr<DbObject> db_ptr;
        if(!enif_get_db(env, argv[0], &db_ptr)) {
            return enif_make_badarg(env);
        }

        rocksdb::OptimisticTransactionDB* db =
            reinterpret_cast<rocksdb::OptimisticTransactionDB*>(db_ptr->m_Db);
        txn->txn = db->BeginTransaction(write_options);
        txn->env = enif_alloc_env();
        ERL_NIF_TERM result = enif_make_resource(env, txn);
        enif_release_resource(txn);
        return enif_make_tuple2(env, ATOM_OK, result);
    }

}
