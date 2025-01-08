cmake_minimum_required(VERSION 3.16)
project(aBinder VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Debug-Konfiguration
set(CMAKE_BUILD_TYPE Debug)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT aBinder)

# Qt-Konfiguration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt-Pfad setzen
set(CMAKE_PREFIX_PATH "C:/Qt/6.8.1/msvc2022_64")
set(QT_PLUGIN_PATH "${CMAKE_PREFIX_PATH}/plugins")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network WebSockets Svg)

# Quellcode-Dateien
set(PROJECT_SOURCES
    src/main.cpp
    src/mainapp/mainwindow.cpp
    src/mainapp/mainwindow.h
    src/login/loginwindow.cpp
    src/login/loginwindow.h
    src/network/socketmanager.cpp
    src/network/socketmanager.h
    src/chat/chatwidget.cpp
    src/chat/chatwidget.h
    src/resources/resources.qrc
)

# Executable
add_executable(aBinder WIN32 ${PROJECT_SOURCES})

# Debug-Informationen aktivieren
if(MSVC)
    target_compile_options(aBinder PRIVATE /Zi)
    set_target_properties(aBinder PROPERTIES 
        LINK_FLAGS "/DEBUG /SUBSYSTEM:CONSOLE"
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endif()

# Qt-Module verlinken
target_link_libraries(aBinder PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::WebSockets
    Qt6::Svg
)

# Installation der Qt-DLLs
if(WIN32)
    # Definiere die zu kopierenden Dateien
    set(QT_DLLS
        "${CMAKE_PREFIX_PATH}/bin/Qt6Core.dll"
        "${CMAKE_PREFIX_PATH}/bin/Qt6Gui.dll"
        "${CMAKE_PREFIX_PATH}/bin/Qt6Widgets.dll"
        "${CMAKE_PREFIX_PATH}/bin/Qt6Network.dll"
        "${CMAKE_PREFIX_PATH}/bin/Qt6WebSockets.dll"
        "${CMAKE_PREFIX_PATH}/bin/Qt6Svg.dll"
    )

    # SSL DLLs
    set(SSL_DLLS
        "${CMAKE_PREFIX_PATH}/bin/libssl-3-x64.dll"
        "${CMAKE_PREFIX_PATH}/bin/libcrypto-3-x64.dll"
        "${CMAKE_PREFIX_PATH}/plugins/tls/qopensslbackend.dll"
    )

    # Debug SSL DLLs
    set(SSL_DEBUG_DLLS
        "${CMAKE_PREFIX_PATH}/plugins/tls/qopensslbackendd.dll"
    )

    # Erstelle Verzeichnisse für Debug und Release
    file(MAKE_DIRECTORY 
        "${CMAKE_CURRENT_BINARY_DIR}/Debug/plugins/platforms"
        "${CMAKE_CURRENT_BINARY_DIR}/Debug/plugins/styles"
        "${CMAKE_CURRENT_BINARY_DIR}/Debug/plugins/imageformats"
        "${CMAKE_CURRENT_BINARY_DIR}/Debug/plugins/iconengines"
        "${CMAKE_CURRENT_BINARY_DIR}/Debug/plugins/tls"
        "${CMAKE_CURRENT_BINARY_DIR}/Release/plugins/platforms"
        "${CMAKE_CURRENT_BINARY_DIR}/Release/plugins/styles"
        "${CMAKE_CURRENT_BINARY_DIR}/Release/plugins/imageformats"
        "${CMAKE_CURRENT_BINARY_DIR}/Release/plugins/iconengines"
        "${CMAKE_CURRENT_BINARY_DIR}/Release/plugins/tls"
    )

    # Kopiere Qt DLLs für Debug und Release
    foreach(DLL_FILE ${QT_DLLS})
        get_filename_component(DLL_NAME ${DLL_FILE} NAME_WE)
        file(COPY ${DLL_FILE} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")
        file(COPY ${DLL_FILE} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Release")
        if(EXISTS "${CMAKE_PREFIX_PATH}/bin/${DLL_NAME}d.dll")
            file(COPY "${CMAKE_PREFIX_PATH}/bin/${DLL_NAME}d.dll" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")
        endif()
    endforeach()

    # Kopiere OpenSSL DLLs für Debug und Release
    foreach(DLL_FILE ${SSL_DLLS})
        file(COPY ${DLL_FILE} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")
        file(COPY ${DLL_FILE} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Release")
    endforeach()

    # Kopiere Debug SSL DLLs
    foreach(DLL_FILE ${SSL_DEBUG_DLLS})
        file(COPY ${DLL_FILE} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")
    endforeach()

    # Kopiere Plugins für Debug und Release
    foreach(CONFIG Debug Release)
        # Plattform-Plugins
        file(COPY "${QT_PLUGIN_PATH}/platforms" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG}/plugins")
        
        # Style-Plugins
        file(COPY "${QT_PLUGIN_PATH}/styles" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG}/plugins")
        
        # Image-Format-Plugins
        file(COPY "${QT_PLUGIN_PATH}/imageformats" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG}/plugins")
        
        # SVG-Icon-Engine
        file(COPY "${QT_PLUGIN_PATH}/iconengines" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG}/plugins")
        
        # TLS Backend
        file(COPY "${QT_PLUGIN_PATH}/tls" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG}/plugins")
    endforeach()

    # Erstelle und kopiere qt.conf für Debug und Release
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/qt.conf" "[Paths]\nPlugins = ./plugins")
    file(COPY "${CMAKE_CURRENT_BINARY_DIR}/qt.conf" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")
    file(COPY "${CMAKE_CURRENT_BINARY_DIR}/qt.conf" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Release")
endif()
