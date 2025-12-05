# Test utilities library
add_library(test_utils STATIC
    utils/ImageComparator.cpp
    utils/ImageHasher.cpp
    utils/TestConfig.cpp
    utils/SceneRegistry.cpp
    utils/BenchmarkRunner.cpp
)

target_include_directories(test_utils PUBLIC
    ${CMAKE_SOURCE_DIR}/src/raymath
    ${CMAKE_SOURCE_DIR}/src/rayimage
    ${CMAKE_SOURCE_DIR}/src/rayscene
    ${CMAKE_SOURCE_DIR}/src/lodepng
    ${CMAKE_SOURCE_DIR}/src/json
    utils/
)

target_link_libraries(test_utils
    raymath
    rayimage
    rayscene
    lodepng
)

# Test executables
add_executable(test_simple_scene tests/test_simple_scene.cpp)
target_link_libraries(test_simple_scene test_utils rayscene raymath rayimage lodepng)
add_test(NAME SimpleSceneTest COMMAND test_simple_scene WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(test_edge_cases tests/test_edge_cases.cpp)
target_link_libraries(test_edge_cases test_utils rayscene raymath rayimage lodepng)
add_test(NAME EdgeCasesTest COMMAND test_edge_cases WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(test_regression tests/test_regression.cpp)
target_link_libraries(test_regression test_utils rayscene raymath rayimage lodepng)
add_test(NAME RegressionTest COMMAND test_regression WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# Utility: compare_with_baseline
add_executable(compare_with_baseline utils/compare_with_baseline.cpp)
target_include_directories(compare_with_baseline PRIVATE ${CMAKE_SOURCE_DIR}/src/json)
target_link_libraries(compare_with_baseline)

