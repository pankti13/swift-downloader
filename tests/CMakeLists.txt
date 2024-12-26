cmake_minimum_required(VERSION 3.10)

project(SwiftDownloaderTests)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(
    ../src/ui
    ../src/core
)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network)

set(SOURCES
    ../src/ui/mainwindow.cpp
    ../src/ui/inputform.cpp
    ../src/core/downloadfile.cpp
    ../src/core/downloadworker.cpp
    MainWindowTest.cpp
)

add_executable(MainWindowTest ${SOURCES})
target_link_libraries(MainWindowTest Qt5::Core Qt5::Widgets Qt5::Network gtest)

find_package(GTest REQUIRED)
target_link_libraries(MainWindowTest GTest::GTest GTest::Main)
enable_testing()
add_test(NAME MainWindowTest COMMAND MainWindowTest)