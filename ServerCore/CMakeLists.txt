cmake_minimum_required(VERSION 3.26)
project(ServerCore)

set(CMAKE_CXX_STANDARD 17)

set(TARGET ServerCore)

set(OUTPUT_DIR ../../lib)

add_library(${TARGET} ServerCore.cpp
        ServerCore.h
        IEventHandler.cpp
        IEventHandler.h
        Socket.cpp
        Socket.h
        Event.cpp
        Event.h
        Queue.cpp
        Queue.h)

set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# include winsock2_32
target_link_libraries(${TARGET} PRIVATE ws2_32)

# include static stdc++ library for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
