#pragma once

#include "wxRwGLContext.h"

class RwGLContext : public wxRwGLContext
{
public:
    RwGLContext(wxGLCanvas* canvas);

protected:
    virtual bool AttachPlugins();
};
