cmake_minimum_required(VERSION 3.26)
project(ChatServer)

set(CMAKE_CXX_STANDARD 17)

set(TARGET ChatServer)
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/../dist/)
set(LIBRARY_HEADER_PATH ${CMAKE_SOURCE_DIR}/../lib/include)
set(PQXX_PATH ${CMAKE_SOURCE_DIR}/../lib/include/libpqxx)
set(SERVERCORE_LIB_PATH ${CMAKE_SOURCE_DIR}/../lib/libServerCore.a)
set(DATABASE_LIB_PATH ${CMAKE_SOURCE_DIR}/../lib/libDatabase.a)

add_executable(${TARGET} main.cpp
        EventHandler.cpp
        EventHandler.h
        Constant.cpp
        Constant.h
        Map.cpp
        Map.h
        Channel.cpp
        Channel.h
        ChannelManager.cpp
        ChannelManager.h
        Client.cpp
        Client.h
        Observer.cpp
        Observer.h)

set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# include ServerCore
target_link_libraries(${TARGET} PRIVATE ${SERVERCORE_LIB_PATH})

# include Database
add_subdirectory(${PQXX_PATH} build-pqxx)
target_link_libraries(${TARGET} PRIVATE pqxx)
target_link_libraries(${TARGET} PRIVATE ${DATABASE_LIB_PATH})

# include headers
target_include_directories(${TARGET} PRIVATE ${LIBRARY_HEADER_PATH})

# include winsock2_32
target_link_libraries(${TARGET} PRIVATE ws2_32)

# include static stdc++ library for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")