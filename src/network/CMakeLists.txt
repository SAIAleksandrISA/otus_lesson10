add_library(bulk_network STATIC
    server.cpp
    server.h
    client_session.cpp
    client_session.h
)

target_include_directories(bulk_network PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(bulk_network PUBLIC bulk_core bulk_common)