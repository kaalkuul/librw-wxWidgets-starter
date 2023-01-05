#pragma once

class MainFrame;
class RwGLContext;

// Define a new application type
class RwStarterApp : public wxApp
{
public:
    RwStarterApp() {}

    void EnableRenderLoop(bool enabled);

    // virtual wxApp methods
    virtual bool OnInit() wxOVERRIDE;
    virtual int OnExit() wxOVERRIDE;

    virtual void OnInitCmdLine(wxCmdLineParser& parser) wxOVERRIDE;
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser) wxOVERRIDE;

    wxFrame* CreateChildFrame(wxView* view);

    RwGLContext* GetContext(wxGLCanvas* canvas);

    wxMenu* AddFileMenu(wxMenuBar* menubar);
    wxMenu* AddEditMenu(wxMenuBar* menubar);
    wxMenu* AddProjectMenu(wxMenuBar* menubar);
    wxMenu* AddHelpMenu(wxMenuBar* menubar);

private:
    void InitialiseDocumentTemplates();

    void CreateAppFrame();
    void OnIdle(wxIdleEvent& evt);

    // append the standard document-oriented menu commands to this menu
    void AppendDocumentFileCommands(wxMenu* menu, bool supportsPrinting);

    // create the edit menu for drawing documents
    wxMenu* CreateDrawingEditMenu();

    // show the about box: as we can have different frames it's more
    // convenient, even if somewhat less usual, to handle this in the
    // application object itself
    void OnAbout(wxCommandEvent& event);

private:
    wxDocManager* m_docManager;
    wxFrame* m_frame;
    bool renderLoopEnabled;
    RwGLContext* m_glContext = { nullptr };
};

DECLARE_APP(RwStarterApp)
