cmake_minimum_required(VERSION 3.10)

project(Swift-Downloader)

set(CMAKE_AUTOMOC ON)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES src/main.cpp src/mainwindow.cpp)
add_executable(Swift-Downloader ${SOURCES})
target_link_libraries(Swift-Downloader Qt5::Core Qt5::Gui Qt5::Widgets)
