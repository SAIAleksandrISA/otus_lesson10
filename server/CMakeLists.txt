cmake_minimum_required(VERSION 3.15)

project(bulk_server_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(bulk_server
    bulk_server.cpp
    server.cpp
    server.h
    client_session.cpp
    client_session.h
)

target_include_directories(bulk_server PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(bulk_server PUBLIC
    Threads::Threads
    async_lib
    bulk_core
    bulk_common
    bulk_threading
)

if(MSVC)
    target_compile_options(bulk_server PRIVATE /W4)
else()
    target_compile_options(bulk_server PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS bulk_server
    RUNTIME DESTINATION bin
)