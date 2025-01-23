cmake_minimum_required(VERSION 3.10)

project(FTPClient VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/FTPClient.cpp
    src/FTPResponseParser.cpp
    src/FTPUtilities.cpp
    src/main.cpp
)

include_directories(include)

add_executable(FTPClientApp ${SOURCES})
