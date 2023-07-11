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
//

#include <memory>

#include "erl_nif.h"

namespace rocksdb {
    class SstFileManager;
}

namespace erocksdb {

  class SstFileManager {
    protected:
      static ErlNifResourceType* m_SstFileManager_RESOURCE;

    public:

      explicit SstFileManager(std::shared_ptr<rocksdb::SstFileManager> mgr);

      ~SstFileManager();

      std::shared_ptr<rocksdb::SstFileManager> sst_file_manager();

      static void CreateSstFileManagerType(ErlNifEnv * Env);
      static void SstFileManagerResourceCleanup(ErlNifEnv *Env, void * Arg);

      static SstFileManager * CreateSstFileManagerResource(std::shared_ptr<rocksdb::SstFileManager> mgr);
      static SstFileManager * RetrieveSstFileManagerResource(ErlNifEnv * Env, const ERL_NIF_TERM & term);

    private:
      std::shared_ptr<rocksdb::SstFileManager> mgr_;
  };

}
