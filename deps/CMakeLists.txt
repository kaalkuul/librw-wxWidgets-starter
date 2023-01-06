include(FetchContent)


find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})


set(LIBRW_PLATFORM "GL3" CACHE STRING "Platform")
set(LIBRW_GL3_GFXLIB "EXTGL3" CACHE STRING "gfxlib for gl3")

FetchContent_Declare(
    librw
    GIT_REPOSITORY https://github.com/kaalkuul/librw.git
    SOURCE_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/librw"
    GIT_TAG        681d137cd97be1ae3c945f03e98fc5e088589997
)


# FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(librw)

add_subdirectory(librw/skeleton)
