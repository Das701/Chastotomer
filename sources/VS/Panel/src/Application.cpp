#pragma warning(push)
#pragma warning(disable:4018 4189 4355 4365 4459 4571 4625 4668 4774 5026 5027)
#include "Application.h"
#include <SDL.h>
#pragma warning(pop)

#undef main


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void update();
extern void init();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0U)
    {
        std::cout << "SDL_Init Error:" << SDL_GetError() << std::endl;
    }

    return wxEntry(argc, argv);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Application::OnInit()
{
    if (!wxApp::OnInit())
    {
        return false;
    }

    init();

    return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Frame::Frame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title)
{
    SetIcon(wxICON(sample));

    wxMenu *fileMenu = new wxMenu;

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Help_About, "&About\tF1", "Show about dialog");

    //fileMenu->Append(File_Size, "&Size", "Resize screen");

    fileMenu->Append(File_Quit, "E&xit\tAlt-X", "Quit this program");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar(2);
    SetStatusText("Welcome to wxWidgets!");

    timer.SetOwner(this, TIMER_ID);

    timer.Start(0);

    timerLongPress.SetOwner(this, TIMER_LONG_ID);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frame::OnTimer(wxTimerEvent&)
{
    update();

    HandlerEvents();

    DrawFPS();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frame::DrawFPS()
{
    static int count = 0;
    static unsigned int prevTime = 0;

    count++;

    if (SDL_GetTicks() - prevTime > 1000)
    {
        float fps = (float)count / (SDL_GetTicks() - prevTime) * 1000.0F;

        char buffer[100];
        sprintf(buffer, "fps %f", fps);

        SetStatusText(buffer);

        prevTime = SDL_GetTicks();
        count = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frame::OnSize(wxCommandEvent&)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
