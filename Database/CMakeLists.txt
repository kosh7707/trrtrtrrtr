cmake_minimum_required(VERSION 3.26)
project(Database)

set(CMAKE_CXX_STANDARD 17)

set(TARGET Database)
set(PQXX_PATH ${CMAKE_SOURCE_DIR}/libpqxx)

set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/../lib)

add_library(${TARGET} STATIC
        include/IDatabase.h
        include/PostgreSQL.h)

set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

target_include_directories(${TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/include/)

file(COPY include DESTINATION ${OUTPUT_DIR}/)
file(COPY libpqxx DESTINATION ${OUTPUT_DIR}/include/)

# include libpqxx
add_subdirectory(${PQXX_PATH} build-pqxx)
target_link_libraries(${TARGET} PRIVATE pqxx)

# include static stdc++ library for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")