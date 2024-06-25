cmake_minimum_required(VERSION 3.22)
project(Ludu)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "Ludu/src/*.cpp")
add_executable("${PROJECT_NAME}" ${SOURCES})

find_package(Vulkan REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})

add_subdirectory("Ludu/external/glfw")
add_subdirectory("Ludu/external/glm")

target_link_libraries("${PROJECT_NAME}"
    glfw
    glm
    ${Vulkan_LIBRARIES}
)