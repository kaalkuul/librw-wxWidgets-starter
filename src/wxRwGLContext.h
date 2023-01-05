#pragma once

class wxRwGLContext : public wxGLContext
{
public:
    wxRwGLContext(wxGLCanvas* canvas);
    virtual ~wxRwGLContext();

    bool InitialiseRw(wxGLCanvas* canvas);
    void TerminateRw();

protected:
    virtual bool PrepareEngineOpenParams(rw::EngineOpenParams& params);
    virtual bool AttachPlugins() = 0;
    virtual void OnRwReady();

private:
    rw::EngineOpenParams engineOpenParams;
    bool rwInitialised;
};
