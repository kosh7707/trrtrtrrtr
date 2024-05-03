cmake_minimum_required(VERSION 3.26)
project(Database)

set(CMAKE_CXX_STANDARD 17)

set(TARGET Database)

set(OUTPUT_DIR ../../lib)

add_library(${TARGET} IDatabase.h
        IDatabase.cpp
        PostgreSQL.h
        PostgreSQL.cpp)

set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# include libpqxx
add_subdirectory(libpqxx build-pqxx)
target_link_libraries(${TARGET} PRIVATE pqxx)

# include static stdc++ library for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")