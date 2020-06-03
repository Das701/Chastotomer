#include "defines.h"
#include "MenuItems.h"
#include "Display/Primitives.h"
#include "Hardware/HAL.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"
#include "Menu/Pages/PageModes.h"
#include "Menu/Pages/PageModesB.h"
#include "Menu/Pages/PageIndication.h"
#include "Utils/String.h"
#include "Settings.h"
#include "Menu/Hint.h"
#include <cstring>
#include <string>
#include "FreqMeter/FreqMeter.h"

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
//    if (Hint::Text())
//    {
        if (CURRENT_CHANNEL_IS_A)  //&& Hint::UnderItem() == PageChannelA::switchTypeSynch
        {
            LEVEL_SYNCH_A = LEVEL_SYNCH_A + delta;
//            Hint::Create(Hint::UnderItem());                 // �������� ����� ���������� ��������� �� ������
//            return true;
        }

        if (CURRENT_CHANNEL_IS_B) //&& Hint::UnderItem() == PageChannelB::switchTypeSynch
        {
            LEVEL_SYNCH_B = LEVEL_SYNCH_B + delta;
//            Hint::Create(Hint::UnderItem());                 // �������� ����� ���������� ��������� �� ������
//            return true;
        }
        
//    }
        return true;
//    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::OnControl(const Control &control)
{
    bool result = false;

    switch (control.value)
    {
    case Control::Right:
        if(PageIndication::calibration.Is(Calibration::Pressed))
        {
            if(SelectedItem())
            {
                result = SelectedItem()->OnControl(control);
            }
        }
        else
        {
            SelectNextItem();
            Hint::Hide();
            result = true;
        }
        info = 1;
        break;

    case Control::Left:
        if(PageIndication::calibration.Is(Calibration::Pressed))
        {
            if(SelectedItem())
            {
                result = SelectedItem()->OnControl(control);
            }
        }
        else
        {
            SelectPrevItem();
            Hint::Hide();
            result = true;
        }
        info = 2;
        break;

    case Control::GovLeft:
//        SelectPrevItem();
//        Hint::Hide();
    if(PageIndication::calibration.Is(Calibration::Pressed))
    {
    }
    else
    {
        result = ChangeLevelSynch(-2);     // ������ ������� �������� ������� �������������
    }
//        if(result == false)
//        {
//            SelectPrevItem();
//            Hint::Hide();
//            result = true;
//        }
//        result = true;
        PLIS::DecreaseN();
        PLIS::WriteData();
        info = 3;
        break;

    case Control::GovRight:
//        SelectNextItem();
//        Hint::Hide();
//        result = true;
    if(PageIndication::calibration.Is(Calibration::Pressed))
    {
    }
    else
    {
        result = ChangeLevelSynch(2);      // ������ ������� �������� ������� �������������
    }
//        if(result == false)
//        {
//            SelectNextItem();
//            Hint::Hide();
//            result = true;
//        }
        PLIS::IncreaseN();
        PLIS::WriteData();
        info = 4;
        break;

    case Control::Enter: 
        if(SelectedItem())
        {
            result = SelectedItem()->OnControl(control);
        }
        info = 5;
        break;

    case Control::Count:
        info = 6;
        break;
    case Control::None:
        info = 7;
        break;
    case Control::Mode:
        if(PageIndication::calibration.Is(Calibration::Pressed))
        {
            if(SelectedItem())
            {
                result = SelectedItem()->OnControl(control);
            }
        }
        if(CURRENT_CHANNEL_IS_A)
        {
            PageModes::PressSetup();
        }
        else if(CURRENT_CHANNEL_IS_B)
        {
            PageModesB::PressSetupB();
        }
        info = 8;
        break;
    case Control::Indication:
        if(PageIndication::calibration.Is(Calibration::Pressed))
        {
            if(SelectedItem())
            {
                result = SelectedItem()->OnControl(control);
            }
        }
        info = 9;
        break;
    case Control::Channels:
        if(PageIndication::calibration.Is(Calibration::Pressed))
        {
            if(SelectedItem())
            {
                result = SelectedItem()->OnControl(control);
            }
        }
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
        if(PageIndication::calibration.Is(Calibration::Pressed))
        {
            if(SelectedItem())
            {
                result = SelectedItem()->OnControl(control);
            }
        }
        else
        {
            PageModes::RelationOff();
            PageModes::InterpoleOff();
            PageModes::DCycleOff();
        }
        info = 12;
        break;
    case Control::Test:
        if(PageIndication::calibration.Is(Calibration::Pressed))
        {
            if(SelectedItem())
            {
                result = SelectedItem()->OnControl(control);
            }
        }
        else
        {
            if((PageModes::modeMeasureFrequency == ModeMeasureFrequency::AC && CURRENT_CHANNEL_IS_A) ||
            (PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::BC && CURRENT_CHANNEL_IS_B))
            {
            }
            else
            {
                FreqMeter::LoadTest();
            }
        }
        info = 13;
        break;
    case Control::Auto:
        if(PageIndication::calibration.Is(Calibration::Pressed))
        {
            if(SelectedItem())
            {
                result = SelectedItem()->OnControl(control);
            }
        }
        else
        {
            PLIS::RefreshAuto();
            FreqMeter::LoadAuto();
            PLIS::SwitchAuto();
        }
        info = 14;
        break;

    default:
        // ������� �������� �� ��������� ����������� �� ���������
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
    if(PageIndication::calibration.Is(Calibration::Pressed))
    {
        if (control.action.IsPress() && (control.value != Control::GovRight || control.value != Control::GovLeft))
        {
            if (Hint::UnderItem() == this)
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
    }
    else
    {
        if (control.action.IsPress() && (control.value == Control::GovButton || control.value == Control::Enter))
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
