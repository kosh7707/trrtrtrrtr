cmake_minimum_required(VERSION 3.26)
project(GameServer)

set(CMAKE_CXX_STANDARD 17)

# Create an executable
add_executable(GameServer main.cpp
        ServerConstant.cpp
        ServerConstant.h
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
        GameSocket.cpp
        GameSocket.h
        Inventory.cpp
        Inventory.h
)

# include ServerCore
add_subdirectory(../ServerCore ServerCore)
target_include_directories(GameServer PRIVATE ../ServerCore)

# include Database
add_subdirectory(../Database Database)
target_include_directories(GameServer PRIVATE ../Database)

# include winsock2_32
target_link_libraries(GameServer PRIVATE ws2_32)

# include static stdc++ library for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")



