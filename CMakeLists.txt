cmake_minimum_required(VERSION 3.15)
project(ConfigLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/ConfigFileLib/resources)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../LogLib/resources)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/test)

add_library(ConfigLib STATIC src/ConfigFile.cpp include/ConfigFile.h)
target_link_libraries(ConfigLib stdc++fs)


