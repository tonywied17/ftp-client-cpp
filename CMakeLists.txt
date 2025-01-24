cmake_minimum_required(VERSION 3.10)

project(FTPClient VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0 
)
FetchContent_MakeAvailable(googletest)


set(SOURCES
    src/FTPClient.cpp
    src/FTPResponseParser.cpp
    src/FTPUtilities.cpp
    main.cpp
)

include_directories(include)

add_executable(FTPClientApp ${SOURCES})

if(WIN32)
    target_link_libraries(FTPClientApp PRIVATE Ws2_32)
endif()

enable_testing()

set(TEST_SOURCES
    tests/FTPClientTest.cpp
    tests/FTPResponseParserTest.cpp
    tests/FTPUtilitiesTest.cpp
)

add_executable(FTPClientTests ${TEST_SOURCES})

target_link_libraries(FTPClientTests gtest gtest_main)

add_test(NAME FTPClientTests COMMAND FTPClientTests)
