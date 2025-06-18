cmake_minimum_required(VERSION 3.16)

project(card)

aux_source_directory(./ SRC)

add_library(card ${SRC})

target_link_libraries(card PRIVATE Qt${QT_VERSION_MAJOR}::Core)
