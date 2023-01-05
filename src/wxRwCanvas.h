#pragma once

class wxRwGLContext;

class wxRwCanvas : public wxGLCanvas
{
    wxDECLARE_EVENT_TABLE();

public:
	wxRwCanvas(wxWindow* parent);
	virtual ~wxRwCanvas();

	bool InitRw();

protected:
	virtual wxRwGLContext* InitContext() = 0;
	virtual void InitRwResources() = 0;
	virtual void Render() = 0;

private:
    void OnPaint(wxPaintEvent& event);
	void OnResized(wxSizeEvent& evt);

private:
	wxRwGLContext* m_glContext;
};
