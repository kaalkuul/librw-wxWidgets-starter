#include "MainFrame.h"
#include "RwStarterDocumentManager.h"
#include "ProjectDocument.h"
#include "ProjectView.h"
#include "RwStarterApp.h"
#include "RwGLContext.h"

#include <wx/persist/toplevel.h>
#include <wx/filefn.h>

bool RwStarterApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    SetVendorName("Kaalkuul");
    SetAppName("RwStarter");
    SetAppDisplayName("RwStarter");

    InitialiseDocumentTemplates();

    // Create main frame window
    CreateAppFrame();

    renderLoopEnabled = false;
    EnableRenderLoop(true);

    if (!wxPersistentRegisterAndRestore(m_frame, "MainFrame"))
    {
        // Choose some custom default size for the first run -- or don't do
        // anything at all and let the system use the default initial size.
        m_frame->SetClientSize(m_frame->FromDIP(wxSize(800, 600)));
    }

    m_frame->Centre();
    m_frame->Show(true);

    m_docManager->CreateNewDocument();

    return true;
}

int RwStarterApp::OnExit()
{
    m_docManager->FileHistorySave(*wxConfig::Get());
    delete m_docManager;

    if (m_glContext)
    {
        delete m_glContext;
    }

    return wxApp::OnExit();
}

void RwStarterApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    wxApp::OnInitCmdLine(parser);
}

bool RwStarterApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    return wxApp::OnCmdLineParsed(parser);
}

wxFrame * RwStarterApp::CreateChildFrame(wxView *view)
{
    wxDocument* doc = view->GetDocument();

    wxString title = doc->GetUserReadableName();

    wxFrame* frame = new wxDocChildFrameAny<wxAuiMDIChildFrame, wxAuiMDIParentFrame>(
        doc,
        view,
        wxStaticCast(GetTopWindow(), wxAuiMDIParentFrame),
        wxID_ANY,
        title, //"Child Frame",
        wxDefaultPosition,
        wxSize(300, 300)
        );

    return frame;
}

void RwStarterApp::EnableRenderLoop(bool enabled)
{
    if (enabled && !renderLoopEnabled)
    {
        Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(RwStarterApp::OnIdle));
        renderLoopEnabled = true;
    }
    else if (!enabled && renderLoopEnabled)
    {
        Disconnect(wxEVT_IDLE, wxIdleEventHandler(RwStarterApp::OnIdle));
        renderLoopEnabled = false;
    }
}

void RwStarterApp::InitialiseDocumentTemplates()
{
    // Create document manager
    m_docManager = new RwStarterDocumentManager;

    // Create a template relating drawing documents to their views
    new wxDocTemplate(m_docManager, "RwStarter project", "*.rwstarter", "", "rwstarter",
        "Project", "Editor View",
        CLASSINFO(ProjectDocument), CLASSINFO(ProjectView));
}

void RwStarterApp::CreateAppFrame()
{
    m_frame = new wxDocParentFrameAny<wxAuiMDIParentFrame>(
        m_docManager, NULL, wxID_ANY,
        GetAppDisplayName(),
        wxDefaultPosition,
        wxSize(500, 400)
        );

    wxMenuBar* menubar = new wxMenuBar;

    AddFileMenu(menubar);
    AddHelpMenu(menubar);

    m_frame->SetMenuBar(menubar);

    //m_frame->SetIcon(wxICON(doc));
}

RwGLContext* RwStarterApp::GetContext(wxGLCanvas* canvas)
{
    if (!m_glContext)
    {
        // Create the OpenGL context for the first window which needs it:
        // subsequently created windows will all share the same context.

        wxGLContextAttrs ctxAttrs;

        m_glContext = new RwGLContext(canvas);

        if (!m_glContext->InitialiseRw(canvas))
        {
            wxLogMessage("Failed to create OpenGL context for LibRW.");
            return nullptr;
        }
    }
    else
    {
        m_glContext->SetCurrent(*canvas);
    }

    return m_glContext;
}

void RwStarterApp::OnIdle(wxIdleEvent& evt)
{
    if (renderLoopEnabled)
    {
        for (wxDocument* doc : m_docManager->GetDocumentsVector())
        {
            for (wxView* view : doc->GetViewsVector())
            {
                ProjectView* projectView = dynamic_cast<ProjectView*>(view);
                if (projectView)
                {
                    projectView->RenderNow();
                }
            }
        }
        
        evt.RequestMore(); // render continuously, not only once on idle
    }
}

void RwStarterApp::AppendDocumentFileCommands(wxMenu* menu, bool supportsPrinting)
{
    menu->Append(wxID_CLOSE);
    menu->Append(wxID_SAVE);
    menu->Append(wxID_SAVEAS);

    if (supportsPrinting)
    {
        menu->AppendSeparator();
        menu->Append(wxID_PRINT);
        menu->Append(wxID_PRINT_SETUP, "Print &Setup...");
        menu->Append(wxID_PREVIEW);
    }
}

wxMenu* RwStarterApp::CreateDrawingEditMenu()
{
    wxMenu* const menu = new wxMenu;
    menu->Append(wxID_UNDO);
    menu->Append(wxID_REDO);
    menu->AppendSeparator();
    menu->Append(wxID_CUT, "&Cut last segment");

    return menu;
}

wxMenu* RwStarterApp::AddFileMenu(wxMenuBar* menubar)
{
    wxMenu* file = new wxMenu;

    file->Append(wxID_NEW);
    file->Append(wxID_OPEN);
    file->AppendSeparator();
    file->Append(wxID_EXIT);

    m_docManager->FileHistoryUseMenu(file);

#if wxUSE_CONFIG
    m_docManager->FileHistoryLoad(*wxConfig::Get());
#endif // wxUSE_CONFIG

    menubar->Append(file, wxGetStockLabel(wxID_FILE));

    return file;
}

wxMenu* RwStarterApp::AddEditMenu(wxMenuBar* menubar)
{
    wxMenu* edit = new wxMenu;
    menubar->Append(edit, wxGetStockLabel(wxID_EDIT));

    return edit;
}

wxMenu* RwStarterApp::AddProjectMenu(wxMenuBar* menubar)
{
    wxMenu* project = new wxMenu;

    menubar->Append(project, _("Project"));

    return project;
}

wxMenu* RwStarterApp::AddHelpMenu(wxMenuBar* menubar)
{
    wxMenu* help = new wxMenu;
    help->Append(wxID_ABOUT);
    menubar->Append(help, wxGetStockLabel(wxID_HELP));

    return help;
}
