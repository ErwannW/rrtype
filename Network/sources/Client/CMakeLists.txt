cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 23)

project(Abn)

include_directories(../../includes)

add_library(abn-client SHARED
    ../../includes/Misc.hpp
    ../../includes/Client.hpp

    Client.cpp
    Is.cpp
    Listen.cpp
    Send.cpp
    SetGet.cpp
)

target_include_directories(abn-client PUBLIC ${ASIO_INCLUDE_DIR})
target_link_libraries(abn-client PUBLIC ${ASIO_LIBS_TO_LINK})

install(DIRECTORY includes DESTINATION include)
install(TARGETS abn-client DESTINATION lib)
