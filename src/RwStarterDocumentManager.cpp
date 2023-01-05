#include "RwStarterDocumentManager.h"

int RwStarterDocumentManager::MakeDocumentNumber()
{
    return m_defaultDocumentNameCounter++;
}

wxString RwStarterDocumentManager::MakeNewDocumentName()
{
    wxDocument* doc = GetCurrentDocument();
    if (doc == nullptr)
        return wxDocManager::MakeNewDocumentName();
    
    wxString name = doc->GetUserReadableName();

    return name;
}
