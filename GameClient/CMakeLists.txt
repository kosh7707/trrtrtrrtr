cmake_minimum_required(VERSION 3.26)
project(GameClient)

set(CMAKE_CXX_STANDARD 17)

add_executable(GameClient main.cpp
        ClientCore.cpp
        ClientCore.h
        ClientConstant.cpp
        ClientConstant.h
        EventHandler.cpp)

# include winsock2_32
target_link_libraries(GameClient PRIVATE ws2_32)

# include static stdc++ library for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")