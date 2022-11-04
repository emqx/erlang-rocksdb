// Copyright (c) 2018-2022 Benoit Chesneau
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


#pragma once

#include <deque>
#include <string>
#include <list>

#include "rocksdb/merge_operator.h"
#include "rocksdb/slice.h"

// forward declaation
namespace rocksdb {
    class MergeOperator;
    class Slice;
    class Logger;
}

namespace erocksdb {

    class ErlangMergeOperator : public rocksdb::MergeOperator {
        public:
            explicit ErlangMergeOperator();

            virtual bool FullMergeV2(
                    const MergeOperationInput& merge_in,
                    MergeOperationOutput* merge_out) const override;

            virtual bool PartialMergeMulti(
                    const rocksdb::Slice& key,
                    const std::deque<rocksdb::Slice>& operand_list,
                    std::string* new_value,
                    rocksdb::Logger* logger) const override;

            virtual const char* Name() const override;

        private:
            bool mergeErlangInt(
                    ErlNifEnv* env,
                    ErlNifSInt64 val,
                    bool next,
                    const MergeOperationInput& merge_in,
                    MergeOperationOutput* merge_out) const;

             bool mergeErlangList(
                    ErlNifEnv* env,
                    std::list<ERL_NIF_TERM> list_in,
                    bool next,
                    const MergeOperationInput& merge_in,
                    MergeOperationOutput* merge_out) const;

            bool mergeErlangBinary(
                    ErlNifEnv* env,
                    std::string s,
                    bool next,
                    const MergeOperationInput& merge_in,
                    MergeOperationOutput* merge_out) const;

    };

    std::shared_ptr<ErlangMergeOperator> CreateErlangMergeOperator();

}
