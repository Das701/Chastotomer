#include "defines.h"
#include "Application_win.h"
#include "GUI/GovernorGUI.h"
#include "Keyboard/Keyboard.h"


const float GovernorGUI::stepDegree = 60.0F;


GovernorGUI::GovernorGUI(wxWindow *parent, const wxPoint &position) : wxPanel(parent, wxID_ANY, position), timer(this, 1)
{
    angleDiscrete = ((float)(std::rand() % 100) - 100.0F) * stepDegree;

    cursor = { false, {0, 0}, 0 };

    SetSize({ radius * 2 + 1, radius * 2 + 1 });
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &GovernorGUI::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &GovernorGUI::OnMouseLeftDown, this);
    Bind(wxEVT_LEFT_UP, &GovernorGUI::OnMouseLeftUp, this);
    Bind(wxEVT_MOTION, &GovernorGUI::OnMouseMove, this);
    Bind(wxEVT_TIMER, &GovernorGUI::OnTimer, this);

    timer.Start(0);
}


void GovernorGUI::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    wxBrush brush({ 0, 0, 0 }, wxTRANSPARENT);
    dc.SetBrush(brush);
   
    dc.DrawCircle(radius, radius, radius);

    float r = static_cast<float>(radius) * 0.6F;

    float x = static_cast<float>(radius) + Sin(angleDiscrete) * r;
    float y = static_cast<float>(radius) + Cos(angleDiscrete) * r;

    dc.DrawCircle(static_cast<int>(x), static_cast<int>(y), radius / 5);
}


void GovernorGUI::OnMouseLeftDown(wxMouseEvent &event)
{
    if(MouseOnGovernor(event))
    {
        needEventPress = true;

        ::SetCursor(LoadCursor(NULL, IDC_HAND));

        cursor.OnPressLeftButton();
    }
}


void GovernorGUI::OnMouseLeftUp(wxMouseEvent &)
{
    if (needEventPress)
    {
        FuncPress();
    }

    needEventPress = false;
}


void GovernorGUI::OnMouseMove(wxMouseEvent &event)
{
    if(MouseOnGovernor(event))
    {
        ::SetCursor(LoadCursor(NULL, IDC_HAND));
    }
}


bool GovernorGUI::MouseOnGovernor(wxMouseEvent &event) //-V2009
{
    int mouseX = 0;
    int mouseY = 0;

    event.GetPosition(&mouseX, &mouseY);

    return radius * radius >= (mouseX - radius) * (mouseX - radius) + (mouseY - radius) * (mouseY - radius);
}


float GovernorGUI::Sin(float degree)
{
    return sinf(static_cast<float>(degree) * 3.1415926F / 180.0F);
}


float GovernorGUI::Cos(float degree)
{
    return cosf(static_cast<float>(degree) * 3.1415926F / 180.0F);
}


void GovernorGUI::OnTimer(wxTimerEvent &)
{
    if(cursor.LeftIsDown())
    {
        ::SetCursor(LoadCursor(NULL, IDC_HAND));

        int delta = cursor.CalculateDelta();

        if(delta != 0)
        {
            needEventPress = false;

            angleFull += (float)(delta * 3);

            if (angleFull <= -60.0F)
            {
                FuncRotate(-1);
                angleFull += stepDegree;
                angleDiscrete -= stepDegree;
                Refresh();
            }

            if (angleFull >= 60.0F)
            {
                FuncRotate(1);
                angleFull -= stepDegree;
                angleDiscrete += stepDegree;
                Refresh();
            }
        }
    }
}


void GovernorGUI::FuncRotate(int delta)
{
    wxCommandEvent event(wxEVT_LEFT_DOWN, delta < 0 ? Control::GovRight : Control::GovLeft);
    Frame::Self()->OnDown(event);
}


void GovernorGUI::FuncPress()
{
    wxCommandEvent event(wxEVT_LEFT_DOWN, Control::GovButton);
    Frame::Self()->OnDown(event);
}


bool GovernorGUI::StructCursor::LeftIsDown()
{
    if(leftIsDown && (::GetKeyState(VK_LBUTTON) != state))
    {
        leftIsDown = false;
    }

    return leftIsDown;
}


int GovernorGUI::StructCursor::CalculateDelta()
{
    POINT pos;

    ::GetCursorPos(&pos);

    int delta = (pos.y - position.y) - (pos.x - position.x);

    position = pos;

    return delta;
}


void GovernorGUI::StructCursor::OnPressLeftButton()
{
    leftIsDown = true;

    ::GetCursorPos(&position);

    state = ::GetKeyState(VK_LBUTTON);
}
