cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 26)

project(Client)
add_executable(Client Client/main.cpp
)
target_include_directories(Client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Client)

project(Server)
add_executable(Server Server/main.cpp
)

target_include_directories(Server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Server)
