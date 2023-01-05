#pragma once

class RwStarterDocumentManager : public wxDocManager
{
public:
	int MakeDocumentNumber();

	virtual wxString MakeNewDocumentName();
};
