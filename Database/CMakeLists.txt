cmake_minimum_required(VERSION 3.26)
project(Database)

set(CMAKE_CXX_STANDARD 17)

add_library(Database IDatabase.h
        IDatabase.cpp
        PostgreSQL.h
        PostgreSQL.cpp)

# include libpqxx
add_subdirectory(libpqxx build-pqxx)
target_link_libraries(Database PRIVATE pqxx)

# include static stdc++ library for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")