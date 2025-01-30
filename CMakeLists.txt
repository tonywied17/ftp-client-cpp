cmake_minimum_required(VERSION 3.10)

project(ftp_client VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(LIBRARY_SOURCES
    include/ftp_library/FTPClient.cpp
    include/ftp_library/FTPResponseParser.cpp
    include/ftp_library/FTPUtilities.cpp
)

add_executable(ftpclient FTPClientApp.cpp ${LIBRARY_SOURCES})

add_library(ftpclient_shared SHARED ${LIBRARY_SOURCES})

target_link_libraries(ftpclient PRIVATE Ws2_32)
target_link_libraries(ftpclient_shared PRIVATE Ws2_32)

set_target_properties(ftpclient PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(ftpclient_shared PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    PREFIX ""
)

add_custom_command(TARGET ftpclient_shared POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:ftpclient_shared>
        ${CMAKE_BINARY_DIR}/lib/ftpclient_shared.dll
)

add_custom_command(TARGET ftpclient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E sleep 5
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/build.ninja
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/ftpclient_shared.dll
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/libftpclient_shared.dll.a
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/.ninja_deps
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/.ninja_log
)
