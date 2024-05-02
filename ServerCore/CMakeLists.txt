cmake_minimum_required(VERSION 3.26)
project(ServerCore)

set(CMAKE_CXX_STANDARD 17)

add_executable(ServerCore main.cpp
        ServerCore.cpp
        ServerCore.h
        IEventHandler.cpp
        IEventHandler.h
        Socket.cpp
        Socket.h
        Event.cpp
        Event.h)

# include winsock2_32
target_link_libraries(ServerCore PRIVATE ws2_32)

# include static stdc++ library for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
