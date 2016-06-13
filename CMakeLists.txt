cmake_minimum_required(VERSION 3.5)
project(jam)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(SQLiteDBProvider SQLiteProvider/Provider.cpp SQLiteProvider/Cursor.cpp SQLiteProvider/Cursor.h SQLiteProvider/SQLite.cpp SQLiteProvider/SQLite.h)
set(SOURCE_FILES main.cpp MainWindow/MainWindow.cpp MainWindow/MainWindow.h ${SQLiteDBProvider} Database/DatabaseIO.cpp Database/DatabaseIO.h Database/DatabaseModels.h QtHelpers/QExtendedStandardItem.h)
add_executable(jam ${SOURCE_FILES})

find_package(Qt5Widgets)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
target_link_libraries(jam Qt5::Widgets)

find_library(SQLITE3_LIBRARY NAMES sqlite3)
target_link_libraries(jam ${SQLITE3_LIBRARY})