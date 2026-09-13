cmake_minimum_required(VERSION 3.15)

project(bulk_server_executable LANGUAGES CXX)

find_package(Boost REQUIRED COMPONENTS system thread)

add_executable(bulk_server
    main.cpp
)

target_link_libraries(bulk_server PRIVATE
    bulk_network
    async_lib
    bulk_core
    bulk_common
    bulk_threading
    Boost::system
    Boost::thread
   
    Threads::Threads
)