#pragma once

#include "RwCanvas.h"

class ProjectDocument;

class ProjectView : public wxView
{
	wxDECLARE_DYNAMIC_CLASS(ProjectView);
	wxDECLARE_EVENT_TABLE();

public:
	ProjectView();

	virtual bool OnCreate(wxDocument* doc, long flags);

	virtual void OnDraw(wxDC* dc);

	virtual void OnUpdate(wxView* sender, wxObject* hint);

	void RenderNow();

private:
	void CreateMenuBar(wxFrame* frame);
	ProjectDocument* GetDocument() const;

private:
	RwCanvas* m_canvas;
};
