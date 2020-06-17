cmake_minimum_required(VERSION 3.3)
project(LearnOpenGL-CLion)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Including GLFW
add_subdirectory(dependencies/glfw)
include_directories(dependencies/glfw/include)

# Including GLAD
include_directories(dependencies/glad/include)

include_directories(${CMAKE_SOURCE_DIR}/includes)

set(SOURCE_FILES src/main.cpp dependencies/glad/src/glad.c includes/learnopengl/shader.h)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# Linking GLFW and OGL
target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})
