cmake_minimum_required(VERSION 3.4.0 FATAL_ERROR)
project(ErlangRocksDBNIF)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

set(priv_dir "${PROJECT_SOURCE_DIR}/../priv")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${priv_dir})

set(ErlangRocksDBNIF_TARGET erocksdb)
add_library(${ErlangRocksDBNIF_TARGET} MODULE "")

set(CMAKE_BUILD_TYPE Release)

# Use lz4 provided by host system
option(ERLANG_ROCKSDB_USE_SYSTEM_LZ4 "Use lz4 from host system" OFF)

# Use snappy provided by host system
option(ERLANG_ROCKSDB_USE_SYSTEM_SNAPPY "Use snappy from host system" OFF)

# Use RocksDB provided by host system
# (for users with extra requirements)
option(ERLANG_ROCKSDB_USE_SYSTEM_ROCKSDB "Use RocksDB from host system" OFF)

if (NOT WIN32)
  # On platforms like FreeBSD where gmake is not the default, we need
  # to look for gmake explicitly to make sure LevelDB can build.
  # TODO(dforsyth): Remove this once LevelDB is upgraded to a version
  # that uses CMake. See MESOS-8757.
  find_program(
    MAKE_PROGRAM
    NAMES gmake make
    DOCS "External 'make' executable.")

  if (NOT MAKE_PROGRAM)
    message(FATAL_ERROR "Could not find an external 'make' executable.")
  endif ()
endif ()

#--------------------------------------------------------------------------------
include(FindErlang)

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${ERLANG_EI_LIBRARY_PATH})

find_library(ERLANG_EI_LIB ei)
find_library(ERLANG_ERL_INTERFACE_LIB erl_interface)
include_directories(${ERLANG_EI_INCLUDE_PATH})
include_directories(${ERLANG_ERTS_INCLUDE_PATH})
#--------------------------------------------------------------------------------

find_package(BZip2)
find_package(ZLIB)
find_package(TBB)

#--------------------------------------------------------------------------------
include(ExternalProject)

if(ERLANG_ROCKSDB_USE_SYSTEM_LZ4)
    find_package(LZ4)
else()
    set(LZ4_ROOT_DIR "${PROJECT_SOURCE_DIR}/../deps/lz4")

    add_custom_command(OUTPUT ${LZ4_ROOT_DIR}/lib/liblz4.a
      COMMAND ${MAKE_PROGRAM} "CFLAGS= -O2 -fPIC"
      WORKING_DIRECTORY ${LZ4_ROOT_DIR}
      COMMENT "Build lz4 library"
      VERBATIM
    )
    add_custom_target(lz4 DEPENDS ${LZ4_ROOT_DIR}/lib/liblz4.a)
    add_dependencies (${ErlangRocksDBNIF_TARGET} lz4)

    set(LZ4_LIBRARY ${LZ4_ROOT_DIR}/lib/liblz4.a)
    set(LZ4_INCLUDE_DIR ${LZ4_ROOT_DIR}/lib/)

    mark_as_advanced(
        LZ4_ROOT_DIR
        LZ4_LIBRARY
        LZ4_INCLUDE_DIR
    )
endif()

#--------------------------------------------------------------------------------

if(ERLANG_ROCKSDB_USE_SYSTEM_SNAPPY)
    find_package(snappy)
else()
    add_dependencies (${ErlangRocksDBNIF_TARGET} snappy)
    set(SNAPPY_ROOT_DIR "${PROJECT_SOURCE_DIR}/../deps/snappy")

    ExternalProject_Add(snappy
        SOURCE_DIR "${SNAPPY_ROOT_DIR}"
        CMAKE_ARGS -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        DOWNLOAD_COMMAND ""
        INSTALL_COMMAND ""
        )

    ExternalProject_Get_Property(snappy BINARY_DIR)
    set(SNAPPY_LIBRARY ${BINARY_DIR}/libsnappy.a)
    set(SNAPPY_INCLUDE_DIR ${SNAPPY_ROOT_DIR})

    mark_as_advanced(
        SNAPPY_ROOT_DIR
        SNAPPY_LIBRARY
        SNAPPY_INCLUDE_DIR
    )
endif()

#--------------------------------------------------------------------------------

if(ERLANG_ROCKSDB_USE_SYSTEM_ROCKSDB)
    find_package(RocksDB 5.17.2 REQUIRED)
else()
    set(ROCKSDB_ROOT_DIR "${PROJECT_SOURCE_DIR}/../deps/rocksdb")
    set(ROCKSDB_EXTRA_CXXFLAGS "${CXXFLAGS} -DNDEBUG -DSNAPPY -DLZ4 -I${SNAPPY_INCLUDE_DIR} -I${LZ4_INCLUDE_DIR} -I${BINARY_DIR} -fPIC")
    set(ROCKSDB_EXTRA_LDFLAGS "${SNAPPY_LIBRARY} ${LZ4_LIBRARY}")

    add_custom_command(OUTPUT ${ROCKSDB_ROOT_DIR}/librocksdb.a
      COMMAND make static_lib EXTRA_CXXFLAGS=${ROCKSDB_EXTRA_CXXFLAGS} EXTRA_LDFLAGS=${ROCKSDB_EXTRA_LDFLAGS} USE_RTTI=1 DISABLE_JEMALLOC=1 ROCKSDB_DISABLE_ZSTD=1 DEBUG_LEVEL=0 PORTABLE=1
      WORKING_DIRECTORY ${ROCKSDB_ROOT_DIR}
      COMMENT "Build RocksDB static library"
      VERBATIM
    )
    add_custom_target(rocksdb DEPENDS ${ROCKSDB_ROOT_DIR}/librocksdb.a)
    add_dependencies(${ErlangRocksDBNIF_TARGET} rocksdb)
    if(NOT ERLANG_ROCKSDB_USE_SYSTEM_SNAPPY)
        add_dependencies (rocksdb snappy)
    endif()
    if(NOT ERLANG_ROCKSDB_USE_SYSTEM_LZ4)
        add_dependencies (rocksdb lz4)
    endif()

    set(ROCKSDB_LIBRARIES "${ROCKSDB_ROOT_DIR}/librocksdb.a")
    set(ROCKSDB_INCLUDE_DIR ${ROCKSDB_ROOT_DIR}/include/)

    unset(ROCKSDB_EXTRA_CXXFLAGS)
    unset(ROCKSDB_EXTRA_LDFLAGS)

    mark_as_advanced(
        ROCKSDB_ROOT_DIR
        ROCKSDB_LIBRARIES
        ROCKSDB_INCLUDE_DIR
    )
endif()

#--------------------------------------------------------------------------------

set_property(TARGET ${ErlangRocksDBNIF_TARGET} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${ErlangRocksDBNIF_TARGET} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${ErlangRocksDBNIF_TARGET} PROPERTY CXX_EXTENSIONS OFF)

target_sources(${ErlangRocksDBNIF_TARGET}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/erocksdb.cc
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/backup.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/batch.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/bitset_merge_operator.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/cache.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/counter_merge_operator.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/env.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/erlang_merge.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/erocksdb_column_family.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/erocksdb_db.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/erocksdb_iter.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/erocksdb_snapshot.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/rate_limiter.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/refobjects.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/sst_file_manager.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/transactions.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/write_buffer_manager.cc
)

target_include_directories(${ErlangRocksDBNIF_TARGET}
    PRIVATE
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${ROCKSDB_INCLUDE_DIR}
    ${LZ4_INCLUDE_DIR}
    ${SNAPPY_INCLUDE_DIR}
    ${PROJECT_BINARY_DIR}
)

target_compile_options(${ErlangRocksDBNIF_TARGET}
    PUBLIC
    -Wall
    -Wextra
    -pedantic-errors
)

set(TARGET_LINKER_OPTIONS)
if(APPLE)
	set(TARGET_LINKER_OPTIONS "-flat_namespace -undefined suppress")
endif(APPLE)

target_link_libraries(${ErlangRocksDBNIF_TARGET}
  PRIVATE
  ${TARGET_LINKER_OPTIONS}
  ${ERLANG_ERL_INTERFACE_LIB}
  ${ERLANG_EI_LIB}
  ${ROCKSDB_LIBRARIES}
  ${LZ4_LIBRARY}
  ${SNAPPY_LIBRARY}
  ${BZIP2_LIBRARIES}
  ${ZLIB_LIBRARY}
  ${TBB_LIBRARIES}
)