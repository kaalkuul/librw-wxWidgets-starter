#pragma once

class MainFrame : public wxFrame
{
public:
    MainFrame();

    void RenderNow();

private:
    void OnClose(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};
