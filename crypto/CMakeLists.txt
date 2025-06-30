cmake_minimum_required(VERSION 3.16)

project(crypto)

aux_source_directory(./ SRC)

file(GLOB HEADERS ./*.h)

add_library(crypto ${SRC} ${HEADERS})

target_link_libraries(crypto PRIVATE Qt${QT_VERSION_MAJOR}::Core OpenSSL::SSL OpenSSL::Crypto)
