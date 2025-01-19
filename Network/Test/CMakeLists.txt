cmake_minimum_required(VERSION 3.24)
project(abn_tu)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(../includes)

add_executable(unit_test
    unit_test.cc

    ../sources/Client/Client.cpp
    ../sources/Client/Is.cpp
    ../sources/Client/Listen.cpp
    ../sources/Client/Send.cpp
    ../sources/Client/SetGet.cpp

    ../sources/Server/Server.cpp
    ../sources/Server/Is.cpp
    ../sources/Server/Listen.cpp
    ../sources/Server/Send.cpp
    ../sources/Server/SetGet.cpp
)
target_link_libraries(unit_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_test)