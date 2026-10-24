cmake_minimum_required(VERSION 3.10)
project (ArgumentParserProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ArgumentParserLib
	Utils/ArgumentParser/ArgumentParser.cpp
)
target_include_directories(ArgumentParserLib PUBLIC
	Utils/ArgumentParser/
)

add_executable(ArgumentParserDemo
	Demo/ArgumentParser/ArgumentParser.cpp
)

target_link_libraries(ArgumentParserDemo PRIVATE
	ArgumentParserLib 
)

add_executable(ReadFileDemo
	Demo/ReadFile/main.cpp
)
target_link_libraries(ReadFileDemo PRIVATE
	ArgumentParserLib
)
add_executable(SimpleCopyDemo
	Demo/SimpleCopy/main.cpp
)
target_link_libraries(SimpleCopyDemo PRIVATE
	ArgumentParserLib
)

add_executable(HackerRmDemo
	Demo/HackerRm/main.cpp
)
target_link_libraries(HackerRmDemo PRIVATE
	ArgumentParserLib
)

