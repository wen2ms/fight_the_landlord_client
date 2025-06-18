cmake_minimum_required(VERSION 3.16)

project(thread)

aux_source_directory(./ SRC)

add_library(thread ${SRC})

target_link_libraries(thread PRIVATE Qt${QT_VERSION_MAJOR}::Core)
