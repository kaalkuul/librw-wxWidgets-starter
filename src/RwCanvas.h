#pragma once

#include "wxRwCanvas.h"

class RwCanvas : public wxRwCanvas
{
    wxDECLARE_EVENT_TABLE();

public:
    RwCanvas(wxWindow* parent);
    virtual ~RwCanvas();

protected:
    virtual wxRwGLContext* InitContext();
    virtual void InitRwResources();
    virtual void Render();

private:
    void OnMouseMoved(wxMouseEvent& event);

private:
    wxPoint m_lastMousePos;
};
