cmake_minimum_required(VERSION 3.26)
project(DatabaseTermProjectClient)

set(CMAKE_CXX_STANDARD 17)

add_executable(DatabaseTermProjectClient main.cpp)

# include winsock2_32
target_link_libraries(DatabaseTermProjectClient ws2_32)

# for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")