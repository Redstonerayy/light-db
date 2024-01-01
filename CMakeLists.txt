# ------------ project stuff ------------ #
CMAKE_MINIMUM_REQUIRED(VERSION 3.24)
project(lightdb)

# ------------ build options ------------ #
set(C_FLAGS "-Wall -std=c17")
set(CXX_FLAGS "-Wall -std=c++20")

# ------------ dependencies ------------ #
find_library(SSL_LIB libssl.a PATHS ./thirdparty/openssl-3.1.4 REQUIRED NO_DEFAULT_PATH)
find_library(CRYPTO_LIB libcrypto.a PATHS ./thirdparty/openssl-3.1.4 REQUIRED NO_DEFAULT_PATH)

find_program(iwyu_path NAMES include-what-you-use iwyu REQUIRED)

# ------------ main executables ------------ #
set(INCLUDE_DIRS src/include thirdparty/openssl-3.1.4/include thirdparty/alpaca/include)
set(LIBS ${SSL_LIB} ${CRYPTO_LIB})

# ------------ server ------------ #
FILE(GLOB SERVER_SOURCE_FILES src/server/*.cpp src/server/tcp_socket_server/*.cpp src/socket/*.cpp src/database/*.cpp)
add_executable(lightdb ${SERVER_SOURCE_FILES})
target_include_directories(lightdb PRIVATE ${INCLUDE_DIRS})
target_link_libraries(lightdb ${LIBS})
set_property(TARGET lightdb PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})

# ------------ client ------------ #
FILE(GLOB CLIENT_SOURCE_FILES src/client/*.cpp src/socket/*.cpp src/database/*.cpp)
add_executable(ldc ${CLIENT_SOURCE_FILES})
target_include_directories(ldc PRIVATE ${INCLUDE_DIRS})
target_link_libraries(ldc ${LIBS})
set_property(TARGET ldc PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})

# ------------ test ------------ #
FILE(GLOB TEST_SOURCE_FILES src/database/*.cpp src/testing/*.cpp)
add_executable(test ${TEST_SOURCE_FILES})
target_include_directories(test PRIVATE ${INCLUDE_DIRS})
set_property(TARGET test PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
