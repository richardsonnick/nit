cmake_minimum_required(VERSION 3.14)

project(nit VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")


set(CTEST_OUTPUT_ON_FAILURE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(nit
  src/main.cpp
)

target_include_directories(nit PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(nit PRIVATE -Wall -Wextra -Wpedantic)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(GTEST_CREATE_SHARED_LIBRARY OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# enable_testing()

# add_executable(nit_tests
#   test/sha1_test.cpp
#   src/hash/sha1.cpp
#   src/util/utils.cpp
# )

# target_include_directories(nit_tests
#   PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/src
#     ${CMAKE_CURRENT_SOURCE_DIR}
# )

# target_link_libraries(nit_tests
#   PRIVATE
#   gtest
#   gtest_main
#   pthread
# )

# include(GoogleTest)
# gtest_discover_tests(nit_tests)

install(TARGETS nit DESTINATION bin)