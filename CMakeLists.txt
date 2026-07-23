cmake_minimum_required(VERSION 3.16)

project(
    SmartPathNavigator
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
    QT NAMES Qt6 Qt5
    REQUIRED COMPONENTS Widgets
)

find_package(
    Qt${QT_VERSION_MAJOR}
    REQUIRED COMPONENTS Widgets
)

# ==========================================
# Project source files
# ==========================================

set(PROJECT_SOURCES

    # Main
    main.cpp

    # Main Window
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    # Graph
    graph/Node.cpp
    graph/Node.h

    graph/Graph.cpp
    graph/Graph.h

    # OSM Parser
    osm/OSMParser.cpp
    osm/OSMParser.h

    # Algorithms
    algorithms/Dijkstra.cpp
    algorithms/Dijkstra.h

    algorithms/AStar.cpp
    algorithms/AStar.h

    # Map Widget
    widgets/MapWidget.cpp
    widgets/MapWidget.h

    # TinyXML2
    xml/tinyxml2.cpp
    xml/tinyxml2.h
)

# ==========================================
# Create executable
# ==========================================

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

    qt_add_executable(
        SmartPathNavigator

        MANUAL_FINALIZATION

        ${PROJECT_SOURCES}
    )

else()

    add_executable(
        SmartPathNavigator

        ${PROJECT_SOURCES}
    )

endif()

# ==========================================
# Qt Libraries
# ==========================================

target_link_libraries(
    SmartPathNavigator

    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)

# ==========================================
# Application properties
# ==========================================

if(${QT_VERSION} VERSION_LESS 6.1.0)

    set(
        BUNDLE_ID_OPTION

        MACOSX_BUNDLE_GUI_IDENTIFIER
        com.example.SmartPathNavigator
    )

endif()

set_target_properties(
    SmartPathNavigator

    PROPERTIES

    ${BUNDLE_ID_OPTION}

    MACOSX_BUNDLE_BUNDLE_VERSION
    ${PROJECT_VERSION}

    MACOSX_BUNDLE_SHORT_VERSION_STRING
    ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}

    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# ==========================================
# Installation
# ==========================================

include(GNUInstallDirs)

install(
    TARGETS SmartPathNavigator

    BUNDLE DESTINATION .

    LIBRARY DESTINATION
    ${CMAKE_INSTALL_LIBDIR}

    RUNTIME DESTINATION
    ${CMAKE_INSTALL_BINDIR}
)

# ==========================================
# Qt 6 Finalization
# ==========================================

if(QT_VERSION_MAJOR EQUAL 6)

    qt_finalize_executable(
        SmartPathNavigator
    )

endif()