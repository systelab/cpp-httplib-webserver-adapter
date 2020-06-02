enable_testing()

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find external dependencides
find_package(WebServerAdapterTestUtilities)

set(GTEST_PACKAGE_NAME gtest)
find_package(${GTEST_PACKAGE_NAME})
if (NOT TARGET ${GTEST_PACKAGE_NAME}::${GTEST_PACKAGE_NAME})
    set(GTEST_PACKAGE_NAME GTest)
    find_package(${GTEST_PACKAGE_NAME})
endif()

# Configure preprocessor definitions
add_compile_options(-D_SCL_SECURE_NO_WARNINGS)
add_compile_options(-DCPPHTTPLIB_OPENSSL_SUPPORT)
add_compile_options(-DCPPHTTPLIB_ZLIB_SUPPORT)

# Configure test project
set(HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT HttpLibWebServerAdapterTest)
file(GLOB_RECURSE HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT_SRC "*.cpp")
file(GLOB_RECURSE HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT_HDR "*.h")
add_executable(${HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT} ${HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT_SRC} ${HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT_HDR})
target_link_libraries(${HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT}
					  HttpLibWebServerAdapter
					  WebServerAdapterTestUtilities::WebServerAdapterTestUtilities
					  ${GTEST_PACKAGE_NAME}::${GTEST_PACKAGE_NAME})

# Ignored missing PDBs link warning on Visual Studio
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set_target_properties(${HTTP_LIB_WEB_SERVER_ADAPTER_TEST_PROJECT} PROPERTIES LINK_FLAGS "/ignore:4099")
endif()

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
