cmake_minimum_required(VERSION 4.0)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

project(SLW)

set(CMAKE_CXX_STANDARD 20)
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

include(SetupLibrary)

add_subdirectory(Containers)
add_subdirectory(UTF8)
add_subdirectory(Base64)
