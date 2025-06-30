cmake_minimum_required(VERSION 3.16)

project(serialize)

find_package(Protobuf)

include_directories(${Protobuf_INCLUDE_DIR})

aux_source_directory(./ SRC)

file(GLOB HEADERS ./*.h)

add_library(serialize ${SRC} ${HEADERS})

target_link_libraries(serialize PRIVATE Qt${QT_VERSION_MAJOR}::Core)
