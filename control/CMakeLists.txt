cmake_minimum_required(VERSION 3.16)

project(control)

aux_source_directory(./ SRC)

add_library(control ${SRC}
    datamanager.h datamanager.cpp)

target_link_libraries(control PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Multimedia)
