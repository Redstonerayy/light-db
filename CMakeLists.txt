# ------------ project stuff ------------ #
CMAKE_MINIMUM_REQUIRED(VERSION 3.24)
project(lightdb)

# ------------ build options ------------ #
set(C_FLAGS "-O2 -Wall -std=c17")
set(CXX_FLAGS "-O2 -Wall -std=c++20")

# ------------ dependencies ------------ #

# ------------ main executable ------------ #
FILE(GLOB SERVER_SOURCE_FILES src/server/*.cpp)
FILE(GLOB CLIENT_SOURCE_FILES src/client/*.cpp)

add_executable(lightdb ${SERVER_SOURCE_FILES})
target_include_directories(lightdb PRIVATE
	src/include
)
target_link_libraries(lightdb PRIVATE
    ssl
    crypto
)

add_executable(ldc ${CLIENT_SOURCE_FILES})
target_include_directories(ldc PRIVATE
	src/include
)
