# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure preprocessor definitions
add_compile_options(-D_SCL_SECURE_NO_WARNINGS)

# Configure test project
set(HTTP_LIB_SAMPLE_WEB_SERVER_PROJECT HttpLibSampleWebServer)
file(GLOB_RECURSE HTTP_LIB_SAMPLE_WEB_SERVER_PROJECT_SRC "*.cpp")
file(GLOB_RECURSE HTTP_LIB_SAMPLE_WEB_SERVER_PROJECT_HDR "*.h")
add_executable(${HTTP_LIB_SAMPLE_WEB_SERVER_PROJECT} ${HTTP_LIB_SAMPLE_WEB_SERVER_PROJECT_SRC} ${HTTP_LIB_SAMPLE_WEB_SERVER_PROJECT_HDR})
target_link_libraries(${HTTP_LIB_SAMPLE_WEB_SERVER_PROJECT} HttpLibWebServerAdapter)

# Ignored missing PDBs link warning on Visual Studio
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set_target_properties(${HTTP_LIB_SAMPLE_WEB_SERVER_PROJECT} PROPERTIES LINK_FLAGS "/ignore:4099")
endif()

#Configure source groups
foreach(FILE ${HTTP_LIB_SAMPLE_WEB_SERVER_PROJECT_SRC} ${HTTP_LIB_SAMPLE_WEB_SERVER_PROJECT_HDR}) 
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
