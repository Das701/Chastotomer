#pragma once
#include <wx/wx.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Frame : public wxFrame
{
public:

    static const int WIDTH = 640;
    static const int HEIGHT = 480;

    Frame(const wxString& title);

    void OnSize(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnTimerLong(wxTimerEvent& event);

private:
    wxTimer timer;
    /// Таймер для "длинного нажатия" кнопок
    wxTimer timerLongPress;

    wxDECLARE_EVENT_TABLE(); //-V2504

    void DrawFPS();
    void HandlerEvents();

public:
    void OnDown(wxCommandEvent &event);
    void OnUp(wxCommandEvent &event);
};
