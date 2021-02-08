#pragma once
#pragma warning(push, 0)
#include <wx/frame.h>
#include <wx/timer.h>
#pragma warning(pop)


class ConsoleSCPI : public wxFrame
{
public:

    static ConsoleSCPI *Self();
    virtual ~ConsoleSCPI();

    void SwitchVisibility();

    void AddLine(const wxString &line);
    void AddText(const wxString &text);

    void StartTest();
    void StopTest();

private:
    ConsoleSCPI(wxFrame *parent);

    void OnSize(wxSizeEvent &);
    void OnTextEnter(wxCommandEvent &);
    void OnTextControlKeyDown(wxKeyEvent &);
    void OnClose(wxCloseEvent &);

    wxTimer timerComPort;
    wxTimer timerTest;
    void OnTimerComPort(wxTimerEvent &);
    void OnTimerTest(wxTimerEvent &);

    void SendToSCPI(const char *);

    class History
    {
    public:
        void Add(const wxString &text);
        wxString Next();
        wxString Prev();

    private:
        std::vector<wxString> history;
        uint position = 1;
    };

    History history;
};
