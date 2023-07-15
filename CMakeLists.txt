cmake_minimum_required(VERSION 3.0.0)
project(7z-assembly VERSION 0.1.0 LANGUAGES CXX)

add_library(7z-assembly ./src/7z-assembly.cpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(CTest)
include(GoogleTest)
enable_testing()
add_executable(tests ./test/7z-assembly.test.cc)
target_link_libraries(tests GTest::gtest_main 7z-assembly)
gtest_discover_tests(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
