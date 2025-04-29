cmake_minimum_required(VERSION 3.10)

project(ftp_client VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")
set(CMAKE_INSTALL_RPATH "$ORIGIN/../../mingw64/bin")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(LIBRARY_SOURCES
    include/ftp_library/FTPClient.cpp
    include/ftp_library/FTPResponseParser.cpp
    include/ftp_library/FTPUtilities.cpp
)

add_executable(ftpclient FTPClientApp.cpp ${LIBRARY_SOURCES})
add_library(ftpclient_shared SHARED ${LIBRARY_SOURCES})

target_include_directories(ftpclient PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(ftpclient_shared PRIVATE ${CMAKE_SOURCE_DIR}/include)

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

target_link_libraries(ftpclient PRIVATE pthread)
target_link_libraries(ftpclient_shared PRIVATE pthread)
