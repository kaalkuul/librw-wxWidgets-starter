#include "RwStarterApp.h"
#include "camera/main.h"
#include "RwGLContext.h"
#include "camera/camexamp.h"
#include "RwCanvas.h"

wxBEGIN_EVENT_TABLE(RwCanvas, wxRwCanvas)
    EVT_MOTION(RwCanvas::OnMouseMoved)
wxEND_EVENT_TABLE()

RwCanvas::RwCanvas(wxWindow* parent)
: wxRwCanvas(parent)
{
    m_lastMousePos = wxGetMousePosition();
}

RwCanvas::~RwCanvas()
{
    Terminate3D();
}

wxRwGLContext* RwCanvas::InitContext()
{
    RwGLContext* context = wxGetApp().GetContext(this);
    return context;
}

void RwCanvas::InitRwResources()
{
    Initialize3D();
}

void RwCanvas::Render()
{
    const wxSize size = GetClientSize() * GetContentScaleFactor();

    rw::Rect rect;
    rect.x = rect.y = 0;
    rect.w = size.GetWidth();
    rect.h = size.GetHeight();

    CameraSizeUpdate(&rect, 0.5f, ((float)rect.h) / rect.w);

    ::Idle(0.01f);

    SwapBuffers();
}

void RwCanvas::OnMouseMoved(wxMouseEvent& event)
{
    wxPoint pos = wxGetMousePosition();
    wxPoint delta = pos - m_lastMousePos;
    m_lastMousePos = pos;

    RotateClump = !event.ControlDown() && !event.AltDown() && !event.ShiftDown() && event.LeftIsDown();
    TranslateClump = !event.ControlDown() && !event.AltDown() && !event.ShiftDown() && event.RightIsDown();
    RotateCamera = event.ControlDown() && event.LeftIsDown();
    TranslateCamera = event.ControlDown() && event.RightIsDown();
    ViewXWindow = event.ShiftDown() && event.LeftIsDown();
    ViewYWindow = event.ShiftDown() && event.RightIsDown();
    ViewXOffset = event.AltDown() && event.LeftIsDown();
    ViewYOffset = event.AltDown() && event.RightIsDown();

    ::MouseMove(delta.x, delta.y);
}
