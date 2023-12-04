cmake_minimum_required(VERSION 3.26)
project(DatabaseTermProject)

set(CMAKE_CXX_STANDARD 17)

# Custom location of the PostgreSQL library
set(PostgreSQL_DIR "C:/Program Files/PostgreSQL/15")
set(PostgreSQL_ROOT "${PostgreSQL_DIR}")
set(PostgreSQL_LIBRARY_DIR "${PostgreSQL_DIR}/lib")
set(PostgreSQL_INCLUDE_DIRS "${PostgreSQL_DIR}/include")
set(PostgreSQL_LIBRARIES "${PostgreSQL_DIR}/lib/libpq.lib")

# Include the INCLUDE directories
include_directories(${PostgreSQL_INCLUDE_DIRS})

# Link the LIBRARY directory
link_directories(${PostgreSQL_LIBRARY_DIRS})

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
        EventHandler.cpp
        EventHandler.h)

# Link the executable `ExamplePostgreSQL` with the PostgreSQL library
target_link_libraries(DatabaseTermProject ${PostgreSQL_LIBRARIES})

# include winsock2_32
target_link_libraries(DatabaseTermProject ws2_32)

# for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")