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

#include <iostream>
#include <string>
#include <memory>
#include <list>
#include <algorithm>
#include <assert.h>

#include "rocksdb/slice.h"
#include "rocksdb/merge_operator.h"

#include "erl_nif.h"
#include "util.h"
#include "counter_merge_operator.h"

namespace erocksdb {

    CounterMergeOperator::CounterMergeOperator() {}

    bool CounterMergeOperator::Merge(
            const rocksdb::Slice& /*key*/,
            const rocksdb::Slice* existing_value,
            const rocksdb::Slice& value,
            std::string* new_value,
            rocksdb::Logger* /*logger*/) const {

        int counter = 0;
        if (existing_value != nullptr) {
            try {
                counter = std::stoi(existing_value->ToString());
            } catch (...) {
                return false;
            }
        }

        int oper;
        try {
            oper = std::stoi(value.ToString());
        } catch(...) {
            return false;
        }
        auto new_counter = counter + oper;
        *new_value = std::to_string(new_counter);
        return true;
    }

    const char* CounterMergeOperator::Name() const  {
        return "CounterMergeOperator";
    }

    std::shared_ptr<CounterMergeOperator> CreateCounterMergeOperator() {
        return std::make_shared<CounterMergeOperator>();
    }
}
