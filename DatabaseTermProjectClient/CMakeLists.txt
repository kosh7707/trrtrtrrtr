cmake_minimum_required(VERSION 3.26)
project(DatabaseTermProjectClient)

set(CMAKE_CXX_STANDARD 17)

add_executable(DatabaseTermProjectClient main.cpp
        ClientCore.cpp
        ClientCore.h
        ClientConstant.cpp
        ClientConstant.h
        DatabaseConnection.cpp
        DatabaseConnection.h
        IDatabaseConnection.cpp
        IDatabaseConnection.h)

# include libpqxx
add_subdirectory(./libpqxx build-pqxx)
target_link_libraries(DatabaseTermProjectClient PRIVATE pqxx)

# include winsock2_32
target_link_libraries(DatabaseTermProjectClient PRIVATE ws2_32)

# include static stdc++ library for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")