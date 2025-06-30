cmake_minimum_required(VERSION 3.16)

project(fight_the_landlord_client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)
find_package(OpenSSL REQUIRED)

set(PROJECT_SOURCES
        main.cpp
)

include_directories(
  ${PROJECT_SOURCE_DIR}/card
  ${PROJECT_SOURCE_DIR}/control
  ${PROJECT_SOURCE_DIR}/strategy
  ${PROJECT_SOURCE_DIR}/windows
  ${PROJECT_SOURCE_DIR}/thread
  ${PROJECT_SOURCE_DIR}/player
  ${PROJECT_SOURCE_DIR}/tcp
  ${PROJECT_SOURCE_DIR}/serialize
  ${PROJECT_SOURCE_DIR}/crypto
  ${OPENSSL_INCLUDE_DIR}
)

add_subdirectory(card)
add_subdirectory(control)
add_subdirectory(strategy)
add_subdirectory(windows)
add_subdirectory(thread)
add_subdirectory(player)
add_subdirectory(tcp)
add_subdirectory(serialize)
add_subdirectory(crypto)


set(MACOSX_BUNDLE_ICON_FILE logo.icns)
set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/images/logo.icns")
set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(fight_the_landlord_client
        MACOSX_BUNDLE 
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${app_icon_macos}
        
        res.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET fight_the_landlord_client APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(fight_the_landlord_client SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(fight_the_landlord_client
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(fight_the_landlord_client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia
  card control strategy windows thread player crypto
)
target_include_directories(fight_the_landlord_client PRIVATE ${PROJECT_SOURCE_DIR})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.fight_the_landlord_client)
endif()
set_target_properties(fight_the_landlord_client PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS fight_the_landlord_client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(fight_the_landlord_client)
endif()
