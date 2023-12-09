cmake_minimum_required(VERSION 3.26)
project(DatabaseTermProject)

set(CMAKE_CXX_STANDARD 17)

# Create an executable
add_executable(DatabaseTermProject main.cpp
        DatabaseConnection.cpp
        DatabaseConnection.h
        ServerConstant.cpp
        ServerConstant.h
        IDatabaseConnection.cpp
        IDatabaseConnection.h
        ServerCore.cpp
        ServerCore.h
        Client.cpp
        Client.h
        Account.cpp
        Account.h
        AccountDAO.cpp
        AccountDAO.h
        InventoryDAO.cpp
        InventoryDAO.h)

# include libpqxx
add_subdirectory(./libpqxx build-pqxx)
target_link_libraries(DatabaseTermProject PRIVATE pqxx)

# include winsock2_32
target_link_libraries(DatabaseTermProject PRIVATE ws2_32)

# include static stdc++ library for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")



