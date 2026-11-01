cmake_minimum_required(VERSION 3.10)
project(DirectoryIteratorDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(UTILS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../) 
add_executable(DirectoryIteratorDemo DirectoryIterator.cpp)

target_include_directories(DirectoryIteratorDemo PRIVATE ${UTILS_DIR})

add_executable(RecursiveDirectoryIteratorDemo RecursiveDirectoryIterator.cpp)
target_include_directories(RecursiveDirectoryIteratorDemo PRIVATE ${UTILS_DIR}) 
