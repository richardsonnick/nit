cmake_minimum_required(VERSION 3.15...4.0)

project(nit VERSION 1.0
  DESCRIPTION "A git-like thing"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Set the output directory to be in build/bin
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
set(CTEST_OUTPUT_ON_FAILURE ON)

find_package(boogielib REQUIRED PATHS /usr/local/lib/boogielib)

## LIBRARY SETUP
add_library(nitlib INTERFACE)
target_include_directories(nitlib INTERFACE src)
target_compile_options(nitlib INTERFACE -Wall -Wextra -Wpedantic)
target_sources(nitlib INTERFACE src/hash-object.h src/FileSystemAdaptor.h src/types.h)

## EXECUTABLE SETUP
add_executable(nit src/main.cpp)
target_link_libraries(nit PRIVATE nitlib)
target_compile_options(nit PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(nit PRIVATE boogielib)

## TESTING SETUP
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(GTEST_CREATE_SHARED_LIBRARY OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(nit_tests
  test/hash_object_test.cpp
  test/FileSystemAdaptorTest.cpp
  test/ObjectStoreTest.cpp
  test/TreeTest.cpp
  test/CommitTest.cpp
  test/IndexTest.cpp
)

target_include_directories(nit_tests
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/test/mocks
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(nit_tests
  PRIVATE
  gtest
  gtest_main
  pthread
  nitlib
  boogielib
)

include(GoogleTest)
gtest_discover_tests(nit_tests)