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
#include "rocksdb/merge_operator.h"
#include "rocksdb/slice.h"

// forward declaation
namespace rocksdb {
    class AssociativeMergeOperator;
}

namespace erocksdb {

    class BitsetMergeOperator : public rocksdb::MergeOperator {
        protected:
            typedef char cell_type;

        public:
            explicit BitsetMergeOperator(unsigned int cap);

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
            unsigned int cap_;
    };

    std::shared_ptr<BitsetMergeOperator> CreateBitsetMergeOperator(unsigned int cap);

}
