cmake_minimum_required(VERSION 3.22.1)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCE_FILES *.cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(Main ${SOURCE_FILES})
target_include_directories(Main AFTER PUBLIC ../lib/)
target_include_directories(Main AFTER PUBLIC .)
target_link_directories(Main BEFORE PUBLIC ../)
target_link_libraries(Main glad glfw3 OpenGL::GL STBImage IMGui)