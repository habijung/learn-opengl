cmake_minimum_required(VERSION 3.20)
project(learn-opengl)

set(CMAKE_CXX_STANDARD 17)
add_compile_definitions(GL_SILENCE_DEPRECATION)

# External - OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
message("OPENGL_INCLUDE_DIR ${OPENGL_INCLUDE_DIR}")

# External - glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(../external/glfw)
include_directories(../external/glfw/include)

# External - assimp
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
add_subdirectory(../external/assimp)
include_directories(../external/assimp/include)

# External - glm
set(GLM_DIR ../external/glm/cmake/glm)
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# External - glad
include_directories(../external/glad/include)
add_library(glad ../external/glad/src/glad.c)

# Targets
add_executable(learn-opengl main.cpp)
target_link_libraries(learn-opengl OpenGL::GL glfw assimp glad)
target_compile_definitions(learn-opengl PUBLIC GLFW_INCLUDE_NONE)