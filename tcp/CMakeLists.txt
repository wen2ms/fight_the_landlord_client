cmake_minimum_required(VERSION 3.16)

project(tcp)

aux_source_directory(./ SRC)

file(GLOB HEADERS ./*.h)

add_library(tcp ${SRC} ${HEADERS})

target_link_libraries(tcp PRIVATE Qt${QT_VERSION_MAJOR}::Core)
