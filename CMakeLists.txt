cmake_minimum_required(VERSION 3.5.0)
project(raytracer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(raytracer main.cpp)

target_include_directories(raytracer PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/src/raymath"
                           "${PROJECT_SOURCE_DIR}/src/rayimage"
                           "${PROJECT_SOURCE_DIR}/src/rayscene"
                           )

add_subdirectory(./src/raymath)
add_subdirectory(./src/rayimage)
add_subdirectory(./src/rayscene)
add_subdirectory(./src/lodepng)

enable_testing()
add_subdirectory(tests)

target_link_libraries(raytracer
                      PUBLIC
                      rayscene
                      raymath
                      rayimage
                      lodepng
                      Threads::Threads
                      )

