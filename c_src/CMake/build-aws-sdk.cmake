add_dependencies(rocksdb aws_sdk)
set(AWS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../deps/aws-sdk-cpp")
set(AWS_ROOT_DIR "${CMAKE_CURRENT_BINARY_DIR}/aws_sdk")
set(AWS_INCLUDE_DIR ${AWS_ROOT_DIR}/include)
set(AWS_LIBRARY ${AWS_ROOT_DIR}/lib)

include(ExternalProject)

ExternalProject_Add(aws_sdk
    SOURCE_DIR "${AWS_SOURCE_DIR}"
    LIST_SEPARATOR |
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${AWS_ROOT_DIR}
        -DBUILD_SHARED_LIBS=OFF
        -DENABLE_TESTING=OFF
        -DBUILD_ONLY=s3|kinesis
    BINARY_DIR ${AWS_ROOT_DIR}
    BUILD_BYPRODUCTS "${AWS_STATIC_LIB}"
    )

set(AWS_FOUND TRUE)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${AWS_ROOT_DIR})
find_package(AWSSDK REQUIRED COMPONENTS s3 kinesis) 

message(STATUS "AWS library: ${AWS_LIBRARY}")
message(STATUS "AWS includes: ${AWS_INCLUDE_DIR}")
message(STATUS "AWS libraries: ${AWS_LIBRARIES}")


mark_as_advanced(
    AWS_ROOT_DIR
    AWS_LIBRARY
    AWS_INCLUDE_DIR
    AWS_LIBRARIES
)

