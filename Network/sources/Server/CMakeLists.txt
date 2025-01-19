cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 23)

project(Abn)

include_directories(../../includes)

add_library(abn-server SHARED
    ../../includes/Misc.hpp
    ../../includes/Server.hpp

    Is.cpp
    Listen.cpp
    Send.cpp
    Server.cpp
    SetGet.cpp
)

target_include_directories(abn-server PUBLIC ${ASIO_INCLUDE_DIR})
target_link_libraries(abn-server PUBLIC ${ASIO_LIBS_TO_LINK})

install(DIRECTORY includes DESTINATION include)
install(TARGETS abn-server DESTINATION lib)
