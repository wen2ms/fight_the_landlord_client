cmake_minimum_required(VERSION 3.16)

project(thread)

aux_source_directory(./ SRC)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(thread ${SRC}
    communication.h communication.cpp
    taskqueue.h taskqueue.cpp)

target_link_libraries(thread PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(thread PRIVATE Qt6::Core)
