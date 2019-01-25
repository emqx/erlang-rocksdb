# Try to find RocksDB headers and library.
#
# Usage of this module as follows:
#
#     find_package(RocksDB)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  ROCKSDB_ROOT_DIR          Set this variable to the root installation of
#                            RocksDB if the module has problems finding the
#                            proper installation path.
#
# Variables defined by this module:
#
#  ROCKSDB_FOUND               System has RocksDB library/headers.
#  ROCKSDB_LIBRARIES           The RocksDB library.
#  ROCKSDB_INCLUDE_DIRS        The location of RocksDB headers.
#  ROCKSDB_VERSION             The RocksDB version

find_path(ROCKSDB_ROOT_DIR
    NAMES include/rocksdb/db.h
)

find_library(ROCKSDB_LIBRARIES
    NAMES rocksdb
    HINTS ${ROCKSDB_ROOT_DIR}/lib
)

find_path(ROCKSDB_INCLUDE_DIRS
    NAMES rocksdb/db.h
    HINTS ${ROCKSDB_ROOT_DIR}/include
)

if (ROCKSDB_ROOT_DIR)
    file(STRINGS ${ROCKSDB_INCLUDE_DIRS}/rocksdb/version.h _ROCKSDB_VERSION_H_CONTENTS REGEX
      "#define[ \t]+ROCKSDB_(MAJOR|MINOR|PATCH)[ \t]+")
    if(_ROCKSDB_VERSION_H_CONTENTS MATCHES "#define[ \t]+ROCKSDB_MAJOR[ \t]+([0-9]+)")
      set(_ROCKSDB_MAJOR_VERSION "${CMAKE_MATCH_1}")
    endif()
    if(_ROCKSDB_VERSION_H_CONTENTS MATCHES "#define[ \t]+ROCKSDB_MINOR[ \t]+([0-9]+)")
      set(_ROCKSDB_MINOR_VERSION "${CMAKE_MATCH_1}")
    endif()
    if(_ROCKSDB_VERSION_H_CONTENTS MATCHES "#define[ \t]+ROCKSDB_PATCH[ \t]+([0-9]+)")
      set(_ROCKSDB_PATCH_VERSION "${CMAKE_MATCH_1}")
    endif()
    set(ROCKSDB_VERSION "${_ROCKSDB_MAJOR_VERSION}.${_ROCKSDB_MINOR_VERSION}.${_ROCKSDB_PATCH_VERSION}")
    unset(_ROCKSDB_MAJOR_VERSION)
    unset(_ROCKSDB_MINOR_VERSION)
    unset(_ROCKSDB_PATCH_VERSION)
    unset(_ROCKSDB_VERSION_H_CONTENTS)
endif(ROCKSDB_ROOT_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RocksDB REQUIRED_VARS
    ROCKSDB_LIBRARIES
    ROCKSDB_INCLUDE_DIRS
    VERSION_VAR
    ROCKSDB_VERSION
)

mark_as_advanced(
    ROCKSDB_ROOT_DIR
    ROCKSDB_LIBRARIES
    ROCKSDB_INCLUDE_DIRS
)