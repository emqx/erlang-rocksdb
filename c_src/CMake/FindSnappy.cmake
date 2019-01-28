# Copyright (c) 2017-present, Facebook, Inc. and its affiliates.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.

find_path(SNAPPY_INCLUDE_DIR NAMES snappy.h)
find_library(SNAPPY_LIBRARY NAMES snappy)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    SNAPPY DEFAULT_MSG
    SNAPPY_LIBRARY SNAPPY_INCLUDE_DIR)


if (SNAPPY_INCLUDE_DIR AND SNAPPY_LIBRARY)
    set(SNAPPY_FOUND TRUE)
    message(STATUS "Found Snappy include: ${SNAPPY_INCLUDE_DIR}")
    message(STATUS "Found Snappy library: ${SNAPPY_LIBRARY}")
endif ()

if (NOT SNAPPY_FOUND)
  message(STATUS "Using third-party bundled Snappy")
endif (NOT SNAPPY_FOUND)

mark_as_advanced(SNAPPY_INCLUDE_DIR SNAPPY_LIBRARY)
