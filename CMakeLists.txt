cmake_minimum_required(VERSION 3.2)

project(HttpLibWebServerAdapter)

# Configure environment
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Configure include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/test)

# Configure Windows version
if (WIN32)
	add_compile_options(-D_WIN32_WINNT=0x0A00)
endif()

# Add subprojects
add_subdirectory(${CMAKE_SOURCE_DIR}/src/HttpLibWebServerAdapter)
add_subdirectory(${CMAKE_SOURCE_DIR}/test/HttpLibWebServerAdapterTest)
add_subdirectory(${CMAKE_SOURCE_DIR}/test/HttpLibSampleWebServer)
