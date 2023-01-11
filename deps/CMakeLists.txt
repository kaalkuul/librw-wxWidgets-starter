include(FetchContent)


find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})


find_package(wxWidgets 3.2.1 CONFIG REQUIRED COMPONENTS html adv gl aui core base)


set(LIBRW_PLATFORM "GL3" CACHE STRING "Platform")
set(LIBRW_GL3_GFXLIB "EXTGL3" CACHE STRING "gfxlib for gl3")

FetchContent_Declare(
    librw
    GIT_REPOSITORY https://github.com/kaalkuul/librw.git
    SOURCE_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/librw"
    GIT_TAG        681d137cd97be1ae3c945f03e98fc5e088589997
)


FetchContent_Declare(
    librw-wx
    GIT_REPOSITORY https://github.com/kaalkuul/librw-wx.git
    SOURCE_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/librw-wx"
    GIT_TAG        4fc4ac934bf7242e6df29928fbb213d229b82e84
)


FetchContent_MakeAvailable(librw)
FetchContent_MakeAvailable(librw-wx)


add_subdirectory(librw/skeleton)
