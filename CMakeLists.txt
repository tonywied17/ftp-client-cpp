cmake_minimum_required(VERSION 3.10)

project(ftp_client VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g -static -static-libgcc -static-libstdc++")

set(WIN_LIBS ole32 comdlg32 oleaut32 uuid Ws2_32 pthread)

set(BUILD_DIR ${CMAKE_BINARY_DIR}/build)
set(BIN_DIR ${BUILD_DIR}/bin)
set(LIB_DIR ${BUILD_DIR}/lib)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/include/ftp_library)
set(LIBRARY_SOURCES
    ${SRC_DIR}/FTPClient.cpp
    ${SRC_DIR}/FTPResponseParser.cpp
    ${SRC_DIR}/FTPUtilities.cpp
)

# CLI Executable
add_executable(ftpclient FTPClientApp.cpp ${LIBRARY_SOURCES})
target_include_directories(ftpclient PRIVATE include)
target_link_libraries(ftpclient PRIVATE ${WIN_LIBS})
set_target_properties(ftpclient PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# GUI Executable
find_package(PkgConfig REQUIRED)
pkg_check_modules(FLTK REQUIRED fltk)
add_executable(ftpclient_gui FTPClientApp-GUI.cpp ${LIBRARY_SOURCES})
target_include_directories(ftpclient_gui PRIVATE include ${FLTK_INCLUDE_DIRS})
target_compile_options(ftpclient_gui PRIVATE ${FLTK_CFLAGS_OTHER})
target_link_libraries(ftpclient_gui PRIVATE ${FLTK_LIBRARIES} ${WIN_LIBS})
set_target_properties(ftpclient_gui PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# Static Library
add_library(ftpclient_shared SHARED ${LIBRARY_SOURCES})
target_include_directories(ftpclient_shared PRIVATE include)
target_link_libraries(ftpclient_shared PRIVATE ${WIN_LIBS})
set_target_properties(ftpclient_shared PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR}
    PREFIX ""
)

add_custom_command(TARGET ftpclient_shared POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:ftpclient_shared>
        ${LIB_DIR}/ftpclient_shared.dll
)
