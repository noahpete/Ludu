# Set where the ImGui files are stored
set(IMGUI_PATH ".")

# Compile as static library 
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp ${IMGUI_PATH}/*.h) 
add_library(imgui STATIC ${IMGUI_SOURCES})

# Set the include directories for ImGui
target_include_directories(imgui PUBLIC ${IMGUI_PATH})

# Link GLFW and Vulkan if they are used internally by ImGui
target_link_libraries(imgui PUBLIC glfw ${Vulkan_LIBRARIES})
