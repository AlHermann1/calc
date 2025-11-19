cmake_minimum_required(VERSION 3.16)

# -------------------
# Project definition
# -------------------
project(calc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------
# Enable Qt automation
# -------------------
set(CMAKE_AUTOUIC ON)   # generates ui_*.h from .ui files
set(CMAKE_AUTOMOC ON)   # handles Q_OBJECT
set(CMAKE_AUTORCC ON)   # handles .qrc files

# -------------------
# Find Qt (works with Qt5 or Qt6)
# -------------------
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# -------------------
# Source files
# -------------------
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    shunter.cpp
    shunter.h
    solver.cpp
    solver.h
    symbol.h
)

# -------------------
# Define executable
# -------------------
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(calc
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        light.qss
        dark.qss
    )
else()
    add_executable(calc ${PROJECT_SOURCES})
endif()

# -------------------
# Link Qt libraries
# -------------------
target_link_libraries(calc PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# -------------------
# Deployment for Windows (bundles Qt DLLs)
# -------------------
if(WIN32 AND QT_VERSION_MAJOR EQUAL 6)
    # Generates a deploy script
    qt_generate_deploy_app_script(
        TARGET calc
        OUTPUT_SCRIPT deploy_script
    )

    # Run this script during installation
    install(SCRIPT ${deploy_script})
endif()

# -------------------
# Set properties for Mac / Windows bundle
# -------------------
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.calc)
endif()

set_target_properties(calc PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# -------------------
# Install rules
# -------------------
include(GNUInstallDirs)

install(TARGETS calc
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}  # .exe on Windows
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # Linux/macOS shared libs
)

# -------------------
# Finalize Qt executable
# -------------------
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(calc)
endif()
