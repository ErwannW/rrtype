cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 23)

project(Abn)

set(CPM_URL "https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake")
set(CPM_PATH "${CMAKE_BINARY_DIR}/CPM.cmake")

message(STATUS "Downloading CPM from ${CPM_URL} to ${CPM_PATH}...")
file (DOWNLOAD
        ${CPM_URL}
        ${CPM_PATH}
        SHOW_PROGRESS
)

include(${CPM_PATH})

CPMAddPackage(NAME asio
        GITHUB_REPOSITORY chriskohlhoff/asio
        GIT_TAG asio-1-32-0)
set(ASIO_INCLUDE_DIR ${asio_SOURCE_DIR}/asio/include)

if (WIN32)
    set (ASIO_LIBS_TO_LINK ws2_32)
elseif ()
    set (ASIO_LIBS_TO_LINK)
endif ()

add_subdirectory(sources/Server)
add_subdirectory(sources/Client)
