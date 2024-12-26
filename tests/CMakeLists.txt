cmake_minimum_required(VERSION 3.10)

project(SwiftDownloaderTests)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(
    ../src/ui
    ../src/core
)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network Test)

set(MainWindowTest_SOURCES
    ../src/ui/mainwindow.cpp
    ../src/ui/inputform.cpp
    ../src/core/downloadfile.cpp 
    ../src/core/downloadworker.cpp
    MainWindowTest.cpp
)

set(DownloadFileTest_SOURCES
    ../src/core/downloadfile.cpp 
    ../src/core/downloadworker.cpp 
    DownloadFileTest.cpp
)

add_executable(MainWindowTest ${MainWindowTest_SOURCES})
add_executable(DownloadFileTest ${DownloadFileTest_SOURCES})

target_link_libraries(MainWindowTest Qt5::Core Qt5::Widgets Qt5::Network gtest Qt5::Test)
target_link_libraries(DownloadFileTest Qt5::Core Qt5::Widgets Qt5::Network gtest Qt5::Test) 

find_package(GTest REQUIRED)
target_link_libraries(MainWindowTest GTest::GTest GTest::Main)
target_link_libraries(DownloadFileTest GTest::GTest GTest::Main)

enable_testing()
add_test(NAME MainWindowTest COMMAND MainWindowTest)
add_test(NAME DownloadFileTest COMMAND DownloadFileTest)