#include "RwStarter.h"
#include "ProjectDocument.h"

wxIMPLEMENT_DYNAMIC_CLASS(ProjectDocument, wxDocument);

ProjectDocument::ProjectDocument()
{
}

ProjectDocument::~ProjectDocument()
{
}

wxString ProjectDocument::GetUserReadableName() const
{
    if (GetFilename().IsEmpty())
        return "New project";

    return wxDocument::GetUserReadableName();
}

bool ProjectDocument::OnNewDocument()
{
    bool result = wxDocument::OnNewDocument();

    return result;
}

std::ostream& ProjectDocument::SaveObject(std::ostream& stream)
{
    wxDocument::SaveObject(stream);

    stream << MAKE_DOCUMENT_VERSION(28, 12, 2022);

    return stream;
}

std::istream& ProjectDocument::LoadObject(std::istream& stream)
{
    wxDocument::LoadObject(stream);

    int version;
    stream >> version;

    return stream;
}
