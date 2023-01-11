#pragma once

class RwGLContext : public wxRwGLContext
{
public:
    RwGLContext(wxGLCanvas* canvas);

protected:
    virtual bool AttachPlugins();
};
