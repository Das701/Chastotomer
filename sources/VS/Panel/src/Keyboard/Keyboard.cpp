#include "defines.h"
#include "Keyboard/Keyboard.h"

#pragma warning(push)
#pragma warning(disable:4365 4668)

#include "Application.h"

#pragma warning(pop)

/// Очередь сообщений - здесь все события органов управления
#define MAX_ACTIONS 100
static Control actions[MAX_ACTIONS];
/// Количество уже имеющихся сообщений
static int numActions = 0;

static bool needStartTimerLong = false;

static bool needStopTimerLong = false;
/// Здесь имя нажатой кнопки
static Control::E pressedKey = Control::_NULL;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Keyboard::Init()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void AddAction(Control control, Control::Action::E action)
{
    if (action != Control::Action::Press)
    {
        return;
    }

    control.action = action;
    actions[numActions++] = control;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frame::OnDown(wxCommandEvent &event)
{
    Control::E key = (Control::E)event.GetId();

    std::cout << "down " << Control(key).Name() << std::endl;
    event.Skip();

    AddAction(key, Control::Action::Press);

    needStartTimerLong = true;

    pressedKey = key;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frame::OnUp(wxCommandEvent &event)
{
    Control::E key = (Control::E)event.GetId();

    std::cout << "up   " << Control(key).Name() << std::endl;
    event.Skip();

    AddAction(key, Control::Action::Release);

    needStopTimerLong = true;

    pressedKey = Control::_NULL;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Keyboard::Empty()
{
    return numActions == 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char *Control::Name()
{
    static const char *names[] =
    {
        "Ручка нажать",
        "РЕЖИМ",
        "ИНДИКАЦИЯ",
        "3",
        "<-",
        "->",
        "6",
        "7",
        "8",
        "9",
        "A",
        "B",
        "C",
        "D",
        "F5",
        "ESC",
        "Left",
        "Right",
        "Ручка влево",
        "Ручка вправо",
        "Channel",
        "ТЕСТ",
        "АВТО",
        "NULL"
    };

    return names[value];
}
