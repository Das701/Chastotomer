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
        for (int i = 0; items[i] != 0; i++)
        {
            if (i == selectedItem)
            {
                Rectangle(WIDTH - 2, HEIGHT - 1).Fill(x + 1, y + 1, Color::_14);
            }
        
            Rectangle(WIDTH, HEIGHT).Draw(x, y, Color::WHITE);
            items[i]->Draw(x, y, i == selectedItem);
            x += Item::WIDTH + 2;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool ChangeLevelSynch(int delta)
{
    if (CURRENT_CHANNEL_IS_A && Hint::FromItem() == PageChannelA::switchTypeSynch)
    {
        LEVEL_SYNCH_A = LEVEL_SYNCH_A + delta;
        Hint::Create(Hint::FromItem());                 // �������� ����� ���������� ��������� �� ������
        return true;
    }

    if (CURRENT_CHANNEL_IS_B && Hint::FromItem() == PageChannelB::switchTypeSynch)
    {
        LEVEL_SYNCH_B = LEVEL_SYNCH_B + delta;
        Hint::Create(Hint::FromItem());                 // �������� ����� ���������� ��������� �� ������
        return true;
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
        SelectNextItem();
        Hint::Hide();
        result = true;
        break;

    case Control::Left:
        SelectPrevItem();
        Hint::Hide();
        result = true;
        break;

    case Control::GovLeft:
        if (Hint::Text())                       // ���� ��������� �� ������
        {
            result = ChangeLevelSynch(-20);     // �� ������ ������� �������� ������� �������������
        }
        break;

    case Control::GovRight:
        if (Hint::Text())                       // ���� ��������� �� ������
        {
            result = ChangeLevelSynch(20);      // �� ������ ������� �������� ������� �������������
        }
        break;

    case Control::GovButton: 
        result = SelectedItem()->OnControl(control);
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
        Math::CircleIncrease<uint8>(&state->value, 0, (uint8)(state->NumStates() - 1));

        if (funcOnPress)
        {
            funcOnPress();
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
    Text(text).Write(x, y + 2, WIDTH);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Switch::CreateHint(char buffer[100]) const
{
    std::strcpy(buffer, hint);
    std::strcat(buffer, " : ");
    std::strcat(buffer, state->ToText());
}
