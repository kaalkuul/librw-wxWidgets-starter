#pragma once

class ProjectDocument : public wxDocument
{
    wxDECLARE_DYNAMIC_CLASS(ProjectDocument);

public:
    ProjectDocument();
    virtual ~ProjectDocument();

    virtual bool OnNewDocument();

    virtual wxString GetUserReadableName() const;

    std::ostream& SaveObject(std::ostream& stream) wxOVERRIDE;
    std::istream& LoadObject(std::istream& stream) wxOVERRIDE;
};
