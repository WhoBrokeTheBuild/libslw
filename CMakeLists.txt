cmake_minimum_required(VERSION 4.0)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

project(SLW)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

    set(CMAKE_CXX_STANDARD 26)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # Disable "#embed is a clang extension" warnings
        add_compile_options(-Wno-c23-extensions)

        # Use the LLVM C++ standard library
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()
    
endif()

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    URL "https://github.com/nlohmann/json/archive/refs/tags/v3.12.0.zip"
    FIND_PACKAGE_ARGS 3.12.0
)
list(APPEND _fetch_content_list nlohmann_json)

include(CTest)
if(BUILD_TESTING)

    set(INSTALL_GTEST OFF)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
    )
    list(APPEND _fetch_content_list googletest)
    
endif()

FetchContent_MakeAvailable(${_fetch_content_list})

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/include/slw/ext/nlohmann/json.hpp)
    file(COPY
        ${nlohmann_json_SOURCE_DIR}/single_include/
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/slw/ext/
    )
endif()

file(GLOB_RECURSE
    _include_list
    "include/**/*.h"
    "include/**/*.hpp"
)

add_library(libslw INTERFACE)

target_sources(
    libslw
    INTERFACE
        ${_include_list}
)

target_include_directories(
    libslw
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB
    _test_source_list
    "tests/*.cpp"
)

foreach(_test_source IN LISTS _test_source_list)

    cmake_path(GET _test_source STEM _test_name)

    add_executable(
        ${_test_name}
        ${_test_source}
    )

    target_link_libraries(
        ${_test_name}
        PRIVATE
            libslw
            GTest::gtest
    )
    
    add_test(
        NAME ${_test_name}
        COMMAND ${_test_name}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )

endforeach()


