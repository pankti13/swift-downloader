cmake_minimum_required(VERSION 3.10)

project(Swift-Downloader)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network)

include_directories(
    src/ui
    src/core
    src/resources
)

set(SOURCES
    src/main.cpp
    src/ui/mainwindow.cpp
    src/ui/inputform.cpp
    src/core/downloadfile.cpp
    src/core/downloadworker.cpp
    src/resources/Swift-Downloader.qrc
)
add_executable(Swift-Downloader ${SOURCES})
target_link_libraries(Swift-Downloader Qt5::Core Qt5::Widgets Qt5::Network)
