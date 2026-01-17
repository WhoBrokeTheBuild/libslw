
macro(setup_library _target)

    file(GLOB_RECURSE
        _public_list
        "Public/**/*.hpp"
    )

    file(GLOB_RECURSE
        _private_list
        "Private/*.cpp"
    )

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

    file(GLOB
        _test_source_list
        "Tests/*Test.cpp"
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
        )
    endforeach()

endmacro()