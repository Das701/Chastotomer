#include "defines.h"
#include "GUI/GovernorGUI.h"


//#define SHOW_NAME_FUNC()   std::cout << __FUNCTION__ << std::endl


GovernorGUI::GovernorGUI(wxWindow *parent, const wxPoint &position) : wxPanel(parent, wxID_ANY, position), timer(this, 1)
{
    angle = static_cast<float>(rand());

    cursor = { false, {0, 0}, 0 };

    SetSize({ radius * 2 + 1, radius * 2 + 1 });
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &GovernorGUI::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &GovernorGUI::OnMouseLeftDown, this);
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

    float x = static_cast<float>(radius) + Sin(static_cast<int>(angle) * stepAngle) * r;
    float y = static_cast<float>(radius) + Cos(static_cast<int>(angle) * stepAngle) * r;

    dc.DrawCircle(static_cast<int>(x), static_cast<int>(y), radius / 5);
}


void GovernorGUI::OnMouseLeftDown(wxMouseEvent &event)
{
    if(MouseOnGovernor(event))
    {
        ::SetCursor(LoadCursor(NULL, IDC_HAND));

        cursor.OnPressLeftButton();
    }
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


float GovernorGUI::Sin(int grad)
{
    return sinf(static_cast<float>(grad) * 3.1415926F / 180.0F);
}


float GovernorGUI::Cos(int grad)
{
    return cosf(static_cast<float>(grad) * 3.1415926F / 180.0F);
}


void GovernorGUI::OnTimer(wxTimerEvent &)
{
    if(cursor.LeftIsDown())
    {
        ::SetCursor(LoadCursor(NULL, IDC_HAND));

        int delta = cursor.CalculateDelta();

        if(delta != 0)
        {
            angle += 0.1F * static_cast<float>(delta);

            Refresh();
        }
    }
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
