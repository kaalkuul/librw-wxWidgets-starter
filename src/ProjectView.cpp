#include "RwStarterApp.h"
#include "ProjectDocument.h"
#include "ProjectView.h"

wxIMPLEMENT_DYNAMIC_CLASS(ProjectView, wxDocument);

wxBEGIN_EVENT_TABLE(ProjectView, wxView)
wxEND_EVENT_TABLE()

ProjectView::ProjectView()
{
}

bool ProjectView::OnCreate(wxDocument* doc, long flags)
{
    if (!wxView::OnCreate(doc, flags))
        return false;

    RwStarterApp& app = wxGetApp();

    wxFrame* frame = app.CreateChildFrame(this);
    wxASSERT(frame == GetFrame());
 
    CreateMenuBar(frame);

    m_canvas = new RwCanvas(frame);
    m_canvas->InitRw();

    frame->Show();

    return true;
}

void ProjectView::CreateMenuBar(wxFrame* frame)
{
    RwStarterApp& app = wxGetApp();

    wxMenuBar* menubar = new wxMenuBar;

    app.AddFileMenu(menubar);
    app.AddEditMenu(menubar);
    app.AddProjectMenu(menubar);
    app.AddHelpMenu(menubar);

    frame->SetMenuBar(menubar);
}

void ProjectView::OnDraw(wxDC* dc)
{
}

void ProjectView::OnUpdate(wxView* sender, wxObject* untypedHint)
{
}

void ProjectView::RenderNow()
{
    m_canvas->Refresh();
}

ProjectDocument* ProjectView::GetDocument() const
{
    return wxStaticCast(wxView::GetDocument(), ProjectDocument);
}
