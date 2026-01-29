cmake_minimum_required(VERSION 4.0)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

project(SLW)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

include(CTest)
if(BUILD_TESTING)

    set(INSTALL_GTEST OFF)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
    )

    FetchContent_MakeAvailable(googletest)
    
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Disable "#embed is a clang extension" warnings
    add_compile_options(-Wno-c23-extensions)
endif()

file(GLOB_RECURSE
    _public_list
    "Public/**/*.h"
    "Public/**/*.hpp"
)

file(GLOB_RECURSE
    _private_list
    "Private/*.h"
    "Private/*.c"
    "Private/*.hpp"
    "Private/*.cpp"
)

add_library(
    SLW
    ${_public_list}
    ${_private_list}
)

target_include_directories(
    SLW
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
)

file(GLOB
    _test_source_list
    "Tests/*.cpp"
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
            SLW
            GTest::gtest
    )
    
    add_test(
        NAME ${_test_name}
        COMMAND ${_test_name}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Tests
    )
endforeach()
