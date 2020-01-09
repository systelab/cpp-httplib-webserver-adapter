enable_testing()

# Configure preprocessor definitions
add_compile_options(-DBOOST_ALL_DYN_LINK -DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find external dependencides
find_package(WebServerAdapterTestUtilities)
find_package(gtest)

# Configure test project
set(HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT HttpLibWebServerAdapterTest)
file(GLOB_RECURSE HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT_SRC "*.cpp")
file(GLOB_RECURSE HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT_HDR "*.h")
add_executable(${HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT} ${HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT_SRC} ${HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT_HDR})
target_link_libraries(${HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT} HttpLibWebServerAdapter WebServerAdapterTestUtilities::WebServerAdapterTestUtilities gtest::gtest)

#Configure source groups
foreach(FILE ${HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT_SRC} ${HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT_HDR}) 
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "Source Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "Header Files${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

# Register tests
add_test(NAME HttpLibWebServerAdapterTest COMMAND ${HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT})

# Additional target for code coverage analysis
if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
	include(CodeCoverage)
	setup_target_for_coverage(HttpLibWebServerAdapterTestCoverage HttpLibWebServerAdapterTest coverage)
endif()
