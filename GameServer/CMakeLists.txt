cmake_minimum_required(VERSION 3.26)
project(GameServer)

set(CMAKE_CXX_STANDARD 17)

set(TARGET GameServer)
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/../dist/)
set(LIBRARY_HEADER_PATH ${CMAKE_SOURCE_DIR}/../lib/include)
set(PQXX_PATH ${CMAKE_SOURCE_DIR}/../lib/include/libpqxx)
set(SERVERCORE_LIB_PATH ${CMAKE_SOURCE_DIR}/../lib/libServerCore.a)
set(DATABASE_LIB_PATH ${CMAKE_SOURCE_DIR}/../lib/libDatabase.a)

# Create an executable
add_executable(${TARGET} main.cpp
        Constants/Constant.cpp
        Constants/Constant.h
        Model/Account.cpp
        Model/Account.h
        DAO/AccountDAO.cpp
        DAO/AccountDAO.h
        DAO/InventoryDAO.cpp
        DAO/InventoryDAO.h
        EventHandler.cpp
        EventHandler.h
        DAO/AuctionDAO.cpp
        DAO/AuctionDAO.h
        Model/Inventory.cpp
        Model/Inventory.h
        Service/InventoryService.h
        Service/InventoryService.cpp
        Service/AccountService.h
        Service/AccountService.cpp
        Observer.cpp
        Observer.h
        Client.cpp
        Client.h
        Model/Item.cpp
        Model/Item.h
)

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



