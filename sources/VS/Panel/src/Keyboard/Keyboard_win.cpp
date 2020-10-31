#include "defines.h"
#include "Keyboard/Keyboard.h"
#include "Application.h"


// ������� ��������� - ����� ��� ������� ������� ����������
#define MAX_ACTIONS 100
static Control actions[MAX_ACTIONS];
// ���������� ��� ��������� ���������
static int numActions = 0;

static bool needStartTimerLong = false;

static bool needStopTimerLong = false;
// ����� ��� ������� ������
static Control::E pressedKey = Control::None;


void Keyboard::Init()
{
}


static void AddAction(Control control, Control::Action::E action)
{
    if (action != Control::Action::Press)
    {
        return;
    }

    control.action = action;
    actions[numActions++] = control;
}


void Frame::OnDown(wxCommandEvent &event)
{
    Control::E key = (Control::E)event.GetId();

    std::cout << "down " << Control(key).Name() << std::endl;
    event.Skip();

    AddAction(key, Control::Action::Press);

    needStartTimerLong = true;

    pressedKey = key;
}


void Frame::OnUp(wxCommandEvent &event)
{
    Control::E key = (Control::E)event.GetId();

    std::cout << "up   " << Control(key).Name() << std::endl;
    event.Skip();

    AddAction(key, Control::Action::Release);

    needStopTimerLong = true;

    pressedKey = Control::None;
}


bool Keyboard::Empty()
{
    return numActions == 0;
}


Control Keyboard::NextControl()
{
    if (Empty())
    {
        return Control();
    }

    Control result = actions[0];

    for (int i = 1; i < numActions; i++)
    {
        actions[i - 1] = actions[i];
    }

    --numActions;

    return result;
}


const char *Control::Name()
{
    static const char *names[Control::E::Count] =
    {
/*  1 */   "����� ������",
/*  2 */   "�����",
/*  3 */   "���������",
/*  4 */   "<-",
/*  5 */   "->",
/*  6 */   "������",
/*  7 */   "ENTER",
/*  8 */   "ESC",
/*  9 */   "����� ����",
/* 10 */   "����� �����",
/* 11 */   "����",
/* 12 */   "����",
/* 13 */   "NULL"
    };

    return names[value];
}
