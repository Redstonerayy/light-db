# ------------ project stuff ------------ #
CMAKE_MINIMUM_REQUIRED(VERSION 3.24)
project(lightdb)

# ------------ build options ------------ #
set(C_FLAGS "-O2 -Wall -std=c17")
set(CXX_FLAGS "-O2 -Wall -std=c++20")

# ------------ dependencies ------------ #
find_library(SSL_LIB libssl.a PATHS ./thirdparty/openssl-3.1.4 REQUIRED NO_DEFAULT_PATH)
find_library(CRYPTO_LIB libcrypto.a PATHS ./thirdparty/openssl-3.1.4 REQUIRED NO_DEFAULT_PATH)

# ------------ main executables ------------ #
set(INCLUDE_DIRS src/include thirdparty/openssl-3.1.4/include thirdparty/alpaca/include)
set(LIBS ${SSL_LIB} ${CRYPTO_LIB})

FILE(GLOB SERVER_SOURCE_FILES src/server/*.cpp src/shared/*.cpp)
add_executable(lightdb ${SERVER_SOURCE_FILES})
target_include_directories(lightdb PRIVATE ${INCLUDE_DIRS})
target_link_libraries(lightdb ${LIBS})

FILE(GLOB CLIENT_SOURCE_FILES src/client/*.cpp src/shared/*.cpp src/database/*.cpp)
add_executable(ldc ${CLIENT_SOURCE_FILES})
target_include_directories(ldc PRIVATE ${INCLUDE_DIRS})
target_link_libraries(ldc ${LIBS})
