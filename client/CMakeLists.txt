cmake_minimum_required(VERSION 3.16)
project(aBinder VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(Qt6_DIR "C:/Qt/6.8.1/msvc2022_64/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets WebSockets Network)

set(PROJECT_SOURCES
    src/main.cpp
    src/login/loginwindow.cpp
    src/login/loginwindow.h
    src/mainapp/mainwindow.cpp
    src/mainapp/mainwindow.h
    src/network/socketmanager.cpp
    src/network/socketmanager.h
    src/resources/resources.qrc
)

add_executable(aBinder WIN32 ${PROJECT_SOURCES})

target_link_libraries(aBinder PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::WebSockets
    Qt6::Network
)

# Windows-spezifische Einstellungen
if(WIN32)
    set_target_properties(aBinder PROPERTIES
        WIN32_EXECUTABLE TRUE
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endif()
