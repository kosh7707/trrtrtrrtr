cmake_minimum_required(VERSION 3.26)
project(GameClient)

set(CMAKE_CXX_STANDARD 17)

set(TARGET GameClient)
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/../dist/)
set(LIBRARY_HEADER_PATH ${CMAKE_SOURCE_DIR}/../lib/include)
set(SERVERCORE_LIB_PATH ${CMAKE_SOURCE_DIR}/../lib/libServerCore.a)

add_executable(GameClient main.cpp
        Constants/Constant.h
        Constants/Constant.cpp
        EventHandler.h
        EventHandler.cpp
        UserInputHandling.cpp
        Constants/ItemConstant.h
        Constants/ItemConstant.cpp)

set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# include ServerCore
target_link_libraries(${TARGET} PRIVATE ${SERVERCORE_LIB_PATH})

# include headers
target_include_directories(${TARGET} PRIVATE ${LIBRARY_HEADER_PATH})

# include winsock2_32
target_link_libraries(GameClient PRIVATE ws2_32)

# include static stdc++ library for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")