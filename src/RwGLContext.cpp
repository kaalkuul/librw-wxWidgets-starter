#include "RwGLContext.h"

RwGLContext::RwGLContext(wxGLCanvas* canvas)
    : wxRwGLContext(canvas)
{
}

bool RwGLContext::AttachPlugins()
{
    rw::registerMeshPlugin();
    rw::registerNativeDataPlugin();
    rw::registerAtomicRightsPlugin();
    rw::registerMaterialRightsPlugin();
    rw::registerSkinPlugin();
    rw::registerUserDataPlugin();
    rw::registerHAnimPlugin();
    rw::registerMatFXPlugin();
    rw::registerUVAnimPlugin();

    return true;
}
