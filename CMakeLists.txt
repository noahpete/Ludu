cmake_minimum_required(VERSION 3.22)
project(Ludu)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Collect source files for Ludu library (excluding headers)
file(GLOB LUDU_SOURCE "Ludu/src/*.cpp" "Ludu/src/**/*.cpp")
add_library("${PROJECT_NAME}_lib" ${LUDU_SOURCE})

# Include directory for Ludu library (for Ludu.h)
target_include_directories("${PROJECT_NAME}_lib"
    PUBLIC "Ludu/src"
)

# Collect source files for Sandbox executable
file(GLOB SANDBOX_SOURCE "Sandbox/src/*.cpp")
add_executable("${PROJECT_NAME}" ${SANDBOX_SOURCE})

# Include directory for Ludu.h in Sandbox executable
target_include_directories("${PROJECT_NAME}"
    PUBLIC "Ludu/src"
)

find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

# Add external dependencies
add_subdirectory("Ludu/external/spdlog")
add_subdirectory("Ludu/external/yaml-cpp")
add_subdirectory("Ludu/external/glfw")
add_subdirectory("Ludu/external/glm")
add_subdirectory("Ludu/external/imgui")
add_subdirectory("Ludu/external/entt")

# Link libraries to the Sandbox executable
target_link_libraries("${PROJECT_NAME}"
    "${PROJECT_NAME}_lib"
    spdlog
    yaml-cpp::yaml-cpp
    glfw
    glm
    ${Vulkan_LIBRARIES}
    imgui
    EnTT
)
