#include "RwStarterApp.h"
#include "RwCanvas.h"
#include "MainFrame.h"

enum
{
    NEW_STEREO_WINDOW = wxID_HIGHEST + 1
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_CLOSE, MainFrame::OnClose)
wxEND_EVENT_TABLE()


MainFrame::MainFrame()
:   wxFrame(NULL, wxID_ANY, _("wxWidgets LibRW starter"))
{
    new EditorGLCanvas(this);

    SetIcon(wxICON(sample));

    // Make a menubar
    wxMenu* menu = new wxMenu;
    menu->Append(wxID_NEW);
    //menu->Append(NEW_STEREO_WINDOW, "New Stereo Window");
    menu->AppendSeparator();
    menu->Append(wxID_CLOSE);
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menu, _("&Cube"));

    SetMenuBar(menuBar);

    CreateStatusBar();

    SetClientSize(400, 400);

    Show();

    // test IsDisplaySupported() function:
    static const int attribs[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };
    wxLogStatus(_L("Double-buffered display %s supported"),
        wxGLCanvas::IsDisplaySupported(attribs) ? "is" : "not");
}

void MainFrame::RenderNow()
{
    Refresh();
}

void MainFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
    wxGetApp().EnableRenderLoop(false);

    // true is to force the frame to close
    Close(true);
}
