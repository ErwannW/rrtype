cmake_minimum_required(VERSION 3.28)
project(Client)

set(CMAKE_CXX_STANDARD 26)
add_executable(Client Client/main.cpp
)

target_include_directories(Client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Client)
