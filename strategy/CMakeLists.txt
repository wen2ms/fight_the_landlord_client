cmake_minimum_required(VERSION 3.16)

project(strategy)

aux_source_directory(./ SRC)

add_library(strategy ${SRC})

target_link_libraries(strategy PRIVATE Qt${QT_VERSION_MAJOR}::Core)
