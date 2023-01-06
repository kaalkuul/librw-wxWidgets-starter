find_package(OpenGL REQUIRED)

find_package(wxWidgets 3.2.1 CONFIG REQUIRED COMPONENTS html adv gl aui core base)

if(WIN32)
    message(STATUS "WXWIN environment set to: $ENV{WXWIN}")
elseif(UNIX)
    set(wxWidgets_USE_UNICODE ON)
    set(wxWidgets_USE_STATIC ON)
endif()

message(STATUS "wxWidgets_USE_FILE = ${wxWidgets_USE_FILE}")
message(STATUS "wxWidgets_LIBRARIES = ${wxWidgets_LIBRARIES}")

# Create a rwstarter executable
# Process manifests for various platforms.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/platform/msw/rwstarter.rc.in ${CMAKE_CURRENT_BINARY_DIR}/rwstarter.rc @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/platform/msw/rwstarter.manifest.in ${CMAKE_CURRENT_BINARY_DIR}/rwstarter.manifest @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/platform/osx/Info.plist.in ${CMAKE_CURRENT_BINARY_DIR}/Info.plist @ONLY)

add_executable(rwstarter WIN32
        wxRwCanvas.h wxRwCanvas.cpp
        wxRwGLContext.h wxRwGLContext.cpp
        main.cpp
        RwStarter.h
        RwStarterApp.h RwStarterApp.cpp
        RwStarterDocumentManager.h RwStarterDocumentManager.cpp
        ProjectDocument.h ProjectDocument.cpp
        ProjectView.h ProjectView.cpp
        RwCanvas.h RwCanvas.cpp
        RwGLContext.h RwGLContext.cpp
        viewer.h viewer.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/rwstarter.rc
    )

target_precompile_headers(rwstarter
  PRIVATE
  <wx/wxprec.h>
  <wx/wx.h>
  <wx/docview.h>
  <wx/docmdi.h>
  <wx/cmdproc.h>
  <wx/aui/tabmdi.h>
  <wx/log.h>
  <wx/config.h>
  <wx/intl.h>
  <wx/cmdline.h>
  <rw.h>
  <wx/glcanvas.h>
)

target_link_libraries(rwstarter
    librw::skeleton
    librw::librw
    ${wxWidgets_LIBRARIES}
)

target_compile_definitions(rwstarter PUBLIC -DwxDEBUG_LEVEL=0)

if (NOT WIN32 AND NOT APPLE)
    # Binary name on unix like systems (Linux, Unix)
    set_target_properties(rwstarter PROPERTIES OUTPUT_NAME "rwstarter")
endif ()

if (APPLE)
#    add_compile_options(-stdlib=libc++)
#    add_definitions(-DBOOST_THREAD_DONT_USE_CHRONO -DBOOST_NO_CXX11_RVALUE_REFERENCES -DBOOST_THREAD_USES_MOVE)
    # -liconv: boost links to libiconv by default
    target_link_libraries(rwstarter "-liconv -framework IOKit" "-framework CoreFoundation" -lc++)
elseif (MSVC)
    # Manifest is provided through rwstarter.rc, don't generate your own.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
else ()
    # Boost on Raspberry-Pi does not link to pthreads explicitely.
    target_link_libraries(rwstarter ${CMAKE_DL_LIBS} -lstdc++ Threads::Threads)
endif ()

if (MSVC)
    # Generate debug symbols even in release mode.
    target_link_options(rwstarter PUBLIC "$<$<CONFIG:RELEASE>:/DEBUG>")
    target_link_libraries(rwstarter user32.lib Setupapi.lib)
elseif (APPLE)
    target_link_libraries(rwstarter "-framework OpenGL")
else ()
    target_link_libraries(rwstarter -ldl)
endif ()

set_target_properties(
    rwstarter PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:rwstarter>")

add_custom_command(
    TARGET rwstarter POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_FILE_DIR:rwstarter>/files"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${PROJECT_SOURCE_DIR}/deps/librw/tools/camera/files" "$<TARGET_FILE_DIR:rwstarter>/files"
)

install(TARGETS rwstarter RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}")
