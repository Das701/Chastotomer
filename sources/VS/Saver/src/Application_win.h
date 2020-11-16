#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)
#include "Display/Display.h"


class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
};


class Frame : public wxFrame
{
public:

    Frame(const wxString& title);

    void OnSize(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnTimerLong(wxTimerEvent& event);

    static Frame *Self();

private:
    wxTimer timer;
    // Таймер для "длинного нажатия" кнопок
    wxTimer timerLongPress;

    wxDECLARE_EVENT_TABLE(); //-V2504

    void DrawFPS();
    void HandlerEvents();

public:
    void OnDown(wxCommandEvent &event);
    void OnUp(wxCommandEvent &event);
    void OnSavePicture(wxCommandEvent &event);

    static Frame *self;
};
