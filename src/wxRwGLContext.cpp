#include "wxRwGLContext.h"

const wxGLContextAttrs* rwGLContextAttrs(wxGLContextAttrs& ctxAttrs)
{
    ctxAttrs.CoreProfile().OGLVersion(3, 3).EndList();
    return &ctxAttrs;
}

wxRwGLContext::wxRwGLContext(wxGLCanvas* canvas)
    : wxGLContext(canvas, NULL, rwGLContextAttrs(wxGLContextAttrs())),
    rwInitialised(false)
{
}

wxRwGLContext::~wxRwGLContext()
{
    TerminateRw();
}

bool wxRwGLContext::InitialiseRw(wxGLCanvas* canvas)
{
    if (!IsOK())
        return false;

    if (!SetCurrent(*canvas))
        return false;

	if (!rw::Engine::init())
		return false;

	if (!AttachPlugins())
	{
		rw::Engine::term();
		return false;
	}

    engineOpenParams.gles = false;
    engineOpenParams.glversion = 33;
    engineOpenParams.windowtitle = "wxWidgets";
    engineOpenParams.loadproc = nullptr;

	if (!PrepareEngineOpenParams(engineOpenParams))
	{
		rw::Engine::term();
		return false;
	}

	if (!rw::Engine::open(&engineOpenParams))
    {
        rw::Engine::term();
        return false;
    }

	if (!rw::Engine::start())
    {
        rw::Engine::close();
        rw::Engine::term();
        return false;
    }

	rw::Charset::open();

	rw::Image::setSearchPath("./");

    rwInitialised = true;

    OnRwReady();

	return true;
}

void wxRwGLContext::TerminateRw()
{
    if (!rwInitialised)
        return;

    rw::Charset::close();

    // TODO: delete all tex dicts
    rw::Engine::stop();
    rw::Engine::close();
    rw::Engine::term();

    rwInitialised = false;
}

bool wxRwGLContext::PrepareEngineOpenParams(rw::EngineOpenParams& params)
{
	return true;
}

void wxRwGLContext::OnRwReady()
{
}
