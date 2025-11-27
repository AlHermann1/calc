message(">>> USING THIS CMAKELISTS <<<")

cmake_minimum_required(VERSION 3.16)

project(calc LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(calc
    src/main.cpp
    src/mainwindow.cpp
    headers/mainwindow.h
    ui/mainwindow.ui
    src/shunter.cpp
    src/solver.cpp
    headers/shunter.h
    headers/solver.h
    headers/symbol.h
)

set_target_properties(calc PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui"
)

# Tell CMake where to look for header files and generated UI headers
target_include_directories(calc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)



target_link_libraries(calc PRIVATE Qt6::Widgets)
