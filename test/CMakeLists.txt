cmake_minimum_required(VERSION 3.14)
project(iot_base_test)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(
  ${PROJECT_NAME}
  ""
)

include_directories(include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../target/drivers/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/FreeRTOS_defs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/fff)

add_subdirectory(src)

target_link_libraries(
  ${PROJECT_NAME}
  GTest::gtest_main
)

gtest_discover_tests(  ${PROJECT_NAME})
