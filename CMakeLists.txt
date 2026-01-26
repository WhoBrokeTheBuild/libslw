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

include(SetupLibrary)

add_subdirectory(Core)
add_subdirectory(IO)
add_subdirectory(Base64)
