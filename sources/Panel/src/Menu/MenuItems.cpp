#include "defines.h"
#include "MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/Pages/PageModes.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"
#include "Utils/String.h"
#include "Settings.h"
#include "Menu/Hint.h"
#include <cstring>
#include <string>


using namespace Display::Primitives;
using namespace Display;

int info = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Enumeration::NumStates() const
{
    int result = 0;

    for (int i = 0; names[i] != nullptr; i++)
    {
        result++;
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Button::OnControl(const Control &)
{
    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Button::Draw(int x, int y, bool selected)
{
    if (selected)
    {
        Color::BLACK.SetAsCurrent();
    }
    Text(text).Write(x, y + 2, WIDTH);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Page::Draw(int x, int y, bool)
{
    if (items)
    {
        for (int i = 0; i < 7; i++)
        {
            
        
            

            if (items[i])
            {
                if (i == selectedItem)
                {
                    Rectangle(WIDTH - 2, HEIGHT - 1).Fill(x + 1, y + 1, Color::_14);
                }
                items[i]->Draw(x, y, i == selectedItem);
                Rectangle(WIDTH, HEIGHT).Draw(x, y, Color::WHITE);
            }

            x += Item::WIDTH + 2;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool ChangeLevelSynch(int delta)
{
    if (Hint::Text())
    {
        if (CURRENT_CHANNEL_IS_A && Hint::UnderItem() == PageChannelA::switchTypeSynch)
        {
            LEVEL_SYNCH_A = LEVEL_SYNCH_A + delta;
            Hint::Create(Hint::UnderItem());                 // Продлить время нахождения подсказки на экране
            return true;
        }

        if (CURRENT_CHANNEL_IS_B && Hint::UnderItem() == PageChannelB::switchTypeSynch)
        {
            LEVEL_SYNCH_B = LEVEL_SYNCH_B + delta;
            Hint::Create(Hint::UnderItem());                 // Продлить время нахождения подсказки на экране
            return true;
        }
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::OnControl(const Control &control)
{
    bool result = false;

    switch (control.value)
    {
    case Control::Right:
        result = ChangeLevelSynch(20);      // Делаем попытку изменить уровень синхронизации
        info = 1;
        break;

    case Control::Left:
        result = ChangeLevelSynch(-20);     // Делаем попытку изменить уровень синхронизации
        info = 2;
        break;

    case Control::GovLeft:
        SelectPrevItem();
        Hint::Hide();
        result = true;
        info = 3;
        break;

    case Control::GovRight:
        SelectNextItem();
        Hint::Hide();
        result = true;
        info = 4;
        break;

    case Control::Enter: 
        info = 5;
        break;

    case Control::Count:
        info = 6;
        break;
    case Control::None:
        info = 7;
        break;
    case Control::Mode:
        info = 8;
        break;
    case Control::Indication:
        info = 9;
        break;
    case Control::Channels:
        info = 10;
        break;
    case Control::GovButton:
        if(SelectedItem())
        {
            result = SelectedItem()->OnControl(control);
        }
        info = 11;
        break;
    case Control::Esc:
        info = 12;
        break;
    case Control::Test:
        info = 13;
        break;
    case Control::Auto:
        info = 14;
        break;

    default:
        // никаких действий по умолчанию производить не требуется
        break;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::NumItems()
{
    int i = 0;
    while(items[i] != 0)
    {
        i++;
    }
    return i;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SelectNextItem()
{
    Math::CircleIncrease<int>(&selectedItem, 0, NumItems() - 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SelectPrevItem()
{
    Math::CircleDecrease<int>(&selectedItem, 0, NumItems() - 1);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Switch::OnControl(const Control &control)
{
    if (control.action.IsPress() && control.value == Control::GovButton)
    {
        if (Hint::Text()[0] != 0 && Hint::UnderItem() == this)
        {
            Math::CircleIncrease<uint8>(&state->value, 0, (uint8)(state->NumStates() - 1));

            if (funcOnPress)
            {
                funcOnPress();
            }
        }

        Hint::Create(this);
        
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Switch::Draw(int x, int y, bool selected)
{
    if(selected)
    {
        Color::BLACK.SetAsCurrent();
    }

    Text(text).WriteInCenterRect(x, y, WIDTH, HEIGHT);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Switch::CreateHint(char buffer[100]) const
{
    std::strcpy(buffer, hint);
    std::strcat(buffer, " : ");
    std::strcat(buffer, state->ToText());
}
