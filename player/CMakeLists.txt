cmake_minimum_required(VERSION 3.16)

project(player)

aux_source_directory(./ SRC)

add_library(player ${SRC})

target_link_libraries(player PRIVATE Qt${QT_VERSION_MAJOR}::Core)
