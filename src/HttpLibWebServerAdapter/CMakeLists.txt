cmake_minimum_required(VERSION 3.2)

# Find external dependencides
find_package(OpenSSL)
find_package(WebServerAdapterInterface)
find_package(ZLIB)

# Configure preprocessor definitions
add_compile_options(-D_SCL_SECURE_NO_WARNINGS)
add_compile_options(-DCPPHTTPLIB_OPENSSL_SUPPORT)
add_compile_options(-DCPPHTTPLIB_ZLIB_SUPPORT)

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure HttpLibWebServerAdapter static library
set(HTTP_LIB_WEB_SERVER_ADAPTER HttpLibWebServerAdapter)
file(GLOB_RECURSE HTTP_LIB_WEB_SERVER_ADAPTER_SRC "*.cpp")
file(GLOB_RECURSE HTTP_LIB_WEB_SERVER_ADAPTER_HDR "*.h")
add_library(${HTTP_LIB_WEB_SERVER_ADAPTER} STATIC ${HTTP_LIB_WEB_SERVER_ADAPTER_SRC} ${HTTP_LIB_WEB_SERVER_ADAPTER_HDR})
target_link_libraries(${HTTP_LIB_WEB_SERVER_ADAPTER} WebServerAdapterInterface::WebServerAdapterInterface OpenSSL::OpenSSL ZLIB::ZLIB)

#Configure source groups
foreach(FILE ${HTTP_LIB_WEB_SERVER_ADAPTER_SRC} ${HTTP_LIB_WEB_SERVER_ADAPTER_HDR}) 
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
