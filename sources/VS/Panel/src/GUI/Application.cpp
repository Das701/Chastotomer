#include "GUI/Application.h"

#undef main


extern void update();
extern void init();



enum
{
    File_Size = wxID_HIGHEST + 1,
    File_Quit = wxID_EXIT,
    Help_About = wxID_ABOUT
};

enum
{
    TIMER_ID = 1,
    TIMER_LONG_ID = 2
};

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_MENU(File_Size, Frame::OnSize)
    EVT_MENU(File_Quit, Frame::OnQuit)
    EVT_MENU(Help_About, Frame::OnAbout)
    EVT_TIMER(TIMER_ID, Frame::OnTimer)
    EVT_TIMER(TIMER_LONG_ID, Frame::OnTimerLong)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP_NO_MAIN(Application);



Frame *Frame::self = nullptr;




int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

    return wxEntry(argc, argv);
}


bool Application::OnInit() //-V2506
{
    if (!wxApp::OnInit())
    {
        return false;
    }

    init();

    return true;
}


Frame::Frame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title)
{
    SetIcon(wxICON(sample));

    wxMenu *fileMenu = new wxMenu;
    //fileMenu->Append(File_Size, "&Size", "Resize screen");
    fileMenu->Append(File_Quit, "E&xit\tAlt-X", "Quit this program");

    wxMenu *toolsMenu = new wxMenu;

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Help_About, "&About\tF1", "Show about dialog");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar(2);
    SetStatusText("Welcome to wxWidgets!");

    timer.SetOwner(this, TIMER_ID);

    timer.Start(0);

    timerLongPress.SetOwner(this, TIMER_LONG_ID);

    self = this;
}


Frame *Frame::Self()
{
    return self;
}


void Frame::OnTimer(wxTimerEvent&)
{
    update();

    HandlerEvents();

    DrawFPS();
}


void Frame::DrawFPS()
{
    static int count = 0;
    static unsigned int prevTime = 0;

    count++;

    if (TIME_MS - prevTime > 1000)
    {
        float fps = (float)count / (float)(TIME_MS - prevTime) * 1000.0F;

        char buffer[100];
        sprintf(buffer, "fps %f", fps);

        SetStatusText(buffer);

        prevTime = TIME_MS;
        count = 0;
    }
}


void Frame::OnSize(wxCommandEvent&)
{

}


void Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}


void Frame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
    (
        "Welcome to %s!\n"
        "\n"
        "This is the minimal wxWidgets sample\n"
        "running under %s.",
        wxVERSION_STRING,
        wxGetOsDescription()
    ),
        "About wxWidgets minimal sample",
        wxOK | wxICON_INFORMATION,
        this);
}
