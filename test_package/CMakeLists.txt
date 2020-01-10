cmake_minimum_required(VERSION 3.2)

project(HttpLibWebServerAdapterPackageTestProject)

# Configure environment
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find external dependencies
find_package(HttpLibWebServerAdapter)

# Configure test package project
set(HTTP_LIB_WEBSERVER_ADAPTER_PACKAGE_TEST_PROJECT HttpLibWebServerAdapterPackageTest)
add_executable(${HTTP_LIB_WEBSERVER_ADAPTER_PACKAGE_TEST_PROJECT} HttpLibWebServerAdapterExample.cpp)
target_link_libraries(${HTTP_LIB_WEBSERVER_ADAPTER_PACKAGE_TEST_PROJECT} HttpLibWebServerAdapter::HttpLibWebServerAdapter)

# Register tests
enable_testing()
add_test(NAME HttpLibWebServerAdapterPackageTest COMMAND ${HTTP_LIB_WEBSERVER_ADAPTER_PACKAGE_TEST_PROJECT})
