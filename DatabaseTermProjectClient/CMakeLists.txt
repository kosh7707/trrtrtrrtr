cmake_minimum_required(VERSION 3.26)
project(DatabaseTermProjectClient)

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

add_executable(DatabaseTermProjectClient main.cpp
        ClientCore.cpp
        ClientCore.h
        ClientConstant.cpp
        ClientConstant.h
        DatabaseConnection.cpp
        DatabaseConnection.h
        IDatabaseConnection.cpp
        IDatabaseConnection.h)

# Link the executable `ExamplePostgreSQL` with the PostgreSQL library
target_link_libraries(DatabaseTermProjectClient ${PostgreSQL_LIBRARIES})

# include winsock2_32
target_link_libraries(DatabaseTermProjectClient ws2_32)

# for distribution
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")