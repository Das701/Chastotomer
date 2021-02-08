#include "GUI/Application.h"
#include "GUI/ConsoleSCPI.h"

#undef main


extern void update();
extern void init();



enum
{
    FILE_SIZE = wxID_HIGHEST + 1,
    FILE_QUIT = wxID_EXIT,
    TOOL_SCPI,
    HELP_ABOUT = wxID_ABOUT
};

enum
{
    TIMER_ID = 1,
    TIMER_LONG_ID = 2
};

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_MENU(FILE_SIZE, Frame::OnSize)
    EVT_MENU(FILE_QUIT, Frame::OnQuit)
    EVT_MENU(HELP_ABOUT, Frame::OnAbout)
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
    fileMenu->Append(FILE_QUIT, "E&xit\tAlt-X", "Quit this program");

    wxMenu *toolsMenu = new wxMenu;
    toolsMenu->Append(TOOL_SCPI, "SCPI");

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(HELP_ABOUT, "&About\tF1", "Show about dialog");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(toolsMenu, "Инструменты");
    menuBar->Append(helpMenu, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar(2);
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &Frame::OnSCPI, this, TOOL_SCPI);

    timer.SetOwner(this, TIMER_ID);

    timer.Start(0);

    timerLongPress.SetOwner(this, TIMER_LONG_ID);

    self = this;

    ConsoleSCPI::Self()->Show();
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


void Frame::OnSCPI(wxCommandEvent &)
{
    ConsoleSCPI::Self()->SwitchVisibility();
}
