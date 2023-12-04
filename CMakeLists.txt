cmake_minimum_required(VERSION 3.0.0)
project(7z-sample-plugin VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17) 
add_library(7z-assembly SHARED ./src/7z-assembly.cc ./src/utils.cc
./src/archive/sz-archive.cc ./src/archive/sze-archive.cc)

include(FetchContent)
FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP NEW
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
option(INSTALL_GTEST OFF)
FetchContent_MakeAvailable(googletest)

include(CTest)
include(GoogleTest)
enable_testing()
add_executable(tests ./test/7z-assembly.test.cc)
target_link_libraries(tests GTest::gtest_main 7z-assembly)
gtest_discover_tests(tests)

set(7Z_ROOT_DIR "C:/Program Files/7-Zip")
set(7Z_PLUGIN_DIR "${7Z_ROOT_DIR}/Formats")
set(CMAKE_INSTALL_PREFIX ${7Z_PLUGIN_DIR})
install(TARGETS 7z-assembly RUNTIME DESTINATION ${7Z_PLUGIN_DIR})
