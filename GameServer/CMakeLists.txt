cmake_minimum_required(VERSION 3.26)
project(GameServer)

set(CMAKE_CXX_STANDARD 17)

# Create an executable
add_executable(GameServer main.cpp
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
        InventoryDAO.h
        EventHandler.cpp
        EventHandler.h
        AuctionDAO.cpp
        AuctionDAO.h
)

# include libpqxx
add_subdirectory(./libpqxx build-pqxx)
target_link_libraries(GameServer PRIVATE pqxx)

# include winsock2_32
target_link_libraries(GameServer PRIVATE ws2_32)

# include static stdc++ library for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")



