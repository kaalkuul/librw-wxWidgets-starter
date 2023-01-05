#include "RwGLContext.h"
#include "wxRwCanvas.h"

wxBEGIN_EVENT_TABLE(wxRwCanvas, wxGLCanvas)
    EVT_PAINT(wxRwCanvas::OnPaint)
    EVT_SIZE(wxRwCanvas::OnResized)
wxEND_EVENT_TABLE()

wxRwCanvas::wxRwCanvas(wxWindow* parent)
// With perspective OpenGL graphics, the wxFULL_REPAINT_ON_RESIZE style
// flag should always be set, because even making the canvas smaller should
// be followed by a paint event that updates the entire canvas with new
// viewport settings.
	: wxGLCanvas(parent, wxID_ANY, NULL,
		wxDefaultPosition, wxDefaultSize,
		wxFULL_REPAINT_ON_RESIZE),
	m_glContext(nullptr)
{
}

wxRwCanvas::~wxRwCanvas()
{
}

bool wxRwCanvas::InitRw()
{
	assert(m_glContext == nullptr);

	m_glContext = InitContext();
	if (!m_glContext)
		return false;

	InitRwResources();

	return true;
}

void wxRwCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	// This is required even though dc is not used otherwise.
    wxPaintDC dc(this);

    // Set the OpenGL viewport according to the client size of this canvas.
    // This is done here rather than in a wxSizeEvent handler because our
    // OpenGL rendering context (and thus viewport setting) is used with
    // multiple canvases: If we updated the viewport in the wxSizeEvent
    // handler, changing the size of one canvas causes a viewport setting that
    // is wrong when next another canvas is repainted.

    const wxSize size = GetClientSize() * GetContentScaleFactor();

	if (m_glContext)
	{
		if (m_glContext->SetCurrent(*this))
		{
			Render();
		}
	}
}

void wxRwCanvas::OnResized(wxSizeEvent& event)
{
}
