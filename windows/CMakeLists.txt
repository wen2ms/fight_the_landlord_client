cmake_minimum_required(VERSION 3.16)

project(windows)

aux_source_directory(./ SRC)

add_library(windows ${SRC})

target_link_libraries(windows PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)
