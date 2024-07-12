include(FetchContent)
cmake_minimum_required(VERSION 3.17)

project(3dog)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
include_directories(src ${OPENGL_INCLUDE_DIRS})

add_subdirectory(vendor/glad)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)

# Set `SOURCES` variable to all .hpp and .cpp files in `src`
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.hpp" "src/*.cpp")

add_executable(3dog ${SOURCES})
target_link_libraries(3dog ${OPENGL_LIBRARIES} glad glfw glm) # Added glad