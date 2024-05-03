cmake_minimum_required(VERSION 3.26)
project(ServerCore)

set(CMAKE_CXX_STANDARD 17)

set(TARGET ServerCore)

set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/../lib)

add_library(${TARGET} STATIC
        src/ServerCore.cpp
        include/ServerCore.h
        src/IEventHandler.cpp
        include/IEventHandler.h
        src/Socket.cpp
        include/Socket.h
        src/Event.cpp
        include/Event.h
        src/Queue.cpp
        include/Queue.h)

set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

target_include_directories(${TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/include/)

file(COPY include DESTINATION ${OUTPUT_DIR}/)

# include winsock2_32
target_link_libraries(${TARGET} PRIVATE ws2_32)

# include static stdc++ library for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
