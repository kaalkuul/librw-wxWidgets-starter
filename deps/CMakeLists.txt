include(FetchContent)


find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        "3.3.8"
    SOURCE_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/glfw"
)


set(LIBRW_PLATFORM "GL3" CACHE STRING "Platform")
set(LIBRW_GL3_GFXLIB "EXTGL3" CACHE STRING "gfxlib for gl3")

FetchContent_Declare(
    librw
    GIT_REPOSITORY https://github.com/kaalkuul/librw.git
    SOURCE_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/librw"
)


# FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(librw)

add_subdirectory(librw/skeleton)
