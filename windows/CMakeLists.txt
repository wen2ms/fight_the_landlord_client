cmake_minimum_required(VERSION 3.16)

project(windows)

aux_source_directory(./ SRC)

add_library(windows ${SRC}
    login.h login.cpp login.ui)

target_link_libraries(windows PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)
