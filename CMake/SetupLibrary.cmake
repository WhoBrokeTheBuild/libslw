
macro(setup_library _target)

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

    if (NOT _private_list)
        add_library(${_target} INTERFACE)

        target_include_directories(
            ${_target}
            INTERFACE
                ${CMAKE_CURRENT_SOURCE_DIR}/Public
        )
    else()
        add_library(
            ${_target}
            ${_public_list}
            ${_private_list}
        )

        target_include_directories(
            ${_target}
            PUBLIC
                ${CMAKE_CURRENT_SOURCE_DIR}/Public
        )
    endif()

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
                ${_target}
                GTest::gtest
        )
        
        add_test(
            NAME ${_test_name}
            COMMAND ${_test_name}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Tests
        )
    endforeach()

endmacro()