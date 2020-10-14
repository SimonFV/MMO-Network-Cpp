cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_STANDARD 14)

project(mmo_network)

set(src
    src/main.cpp
)

include_directories(
    include
    C:/asio-1.18.0/include/
)

option(MINGW_STDTHREADS_GENERATE_STDHEADERS "" ON)
add_subdirectory(mingw-std-threads)


add_executable(run ${src})

if(MINGW)
  target_link_libraries(run ws2_32 mingw_stdthreads)
endif()