#include "defines.h"
#include "FreqMeter.h"
#include "Settings.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Hint.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Settings/PagesSettings.h"
#include "Menu/Pages/PageIndication.h"
#include "Utils/Math.h"
#include <cstring>


using namespace Display::Primitives;
using namespace Display;



int Enumeration::NumStates() const
{
    int result = 0;

    if (correct == nullptr)
    {
        for (int i = 0; names[i] != nullptr; i++)
        {
            result++;
        }
    }
    else
    {
        return numStates;
    }

    return result;
}


char *Enumeration::ToText() const
{
    if (correct == nullptr)
    {
        return names[value];
    }

    int counter = -1;

    for (int i = 0; i <= value; i++)
    {
        if (correct[i])
        {
            counter++;
        }
    }

    return names[counter];
}


char *Enumeration::UGO() const
{
    if (correct == nullptr)
    {
        return ugo[value];
    }

    int counter = -1;

    for (int i = 0; i <= value; i++)
    {
        if (correct[i])
        {
            counter++;
        }
    }

    return ugo[value];
}


bool Button::OnControl(const Control &)
{
    funcOnPress();

    return true;
}


void Page::Draw(int x, int y, int, bool)
{
    if (Display::InDrawingPart(y, HEIGHT))
    {
        for (int i = 0; i < NumItems(); i++)
        {
            bool selected = (i == selectedItem);

            Rectangle(WidthItem(i) - 1, HEIGHT - 1).FillRounded(x, y + 1, 2, ColorBackground(selected), Color::WHITE);

            items[i]->Draw(x, y + 5, WidthItem(i), selected);

            x += WidthItem(i);
        }
    }
}


int Page::WidthItem(int num) const
{
    if (IsPageModes())
    {
        if (num == 0)
        {
            return ((Display::WIDTH - 1) / 6) * 2 - 50;
        }
    }

    int result = ((num % 2) == 0) ? ((Display::WIDTH - 1) / 6) : (Display::WIDTH / 6);

    if (num == 5)
    {
        result += 3;
    }

    return result;
}


bool Page::IsPageModes() const
{
    return (this == PageModesA::self) || (this == PageModesB::self) || (this == PageModesC::self) || (this == PageModesD::self);
}


bool Page::IsPageSettings() const
{
    return (this == PageSettingsA::self) || (this == PageSettingsB::self) || (this == PageSettingsC::self) || (this == PageSettingsD::self);
}


int Page::NumItems() const
{
    int i = 0;
    while (items[i] != nullptr)
    {
        i++;
    }
    return i;
}


void Page::SelectNextItem()
{
    Math::CircleIncrease<int>(&selectedItem, 0, NumItems() - 1);
}


void Page::SelectPrevItem()
{
    Math::CircleDecrease<int>(&selectedItem, 0, NumItems() - 1);
}


void Page::VerifySelectedItem()
{
    if (selectedItem >= NumItems())
    {
        selectedItem = NumItems() - 1;
    }
}


void Switch::NextChoice()
{
    if (state->correct == nullptr)
    {
        Math::CircleIncrease<uint8>(&state->value, 0, (uint8)(state->NumStates() - 1));
    }
    else
    {
        do 
        {
            Math::CircleIncrease<uint8>(&state->value, 0, (uint8)(state->NumStates() - 1));
        } while (state->correct[state->value] == false);
    }

    if (funcOnPress)
    {
        funcOnPress();
    }
}


bool Switch::OnControl(const Control &control)
{
    if (PageIndication::calibration.Is(Calibration::Pressed))
    {
        if (control.action.IsPress() && (control.value != Control::GovRight || control.value != Control::GovLeft)) //-V560
        {
            if (Hint::UnderItem() == this)
            {
                NextChoice();
            }

            Hint::Create(this);

            return true;
        }
    }
    else
    {
        if (control.action.IsPress() && (control.value == Control::GovButton))
        {

            if (Hint::Text()[0] != 0 && Hint::UnderItem() == this)
            {
                NextChoice();
            }

            Hint::Create(this);

            return true;
        }

        if (control.action.IsPress() && (control.value == Control::Enter))
        {

            if (PageIndication::launchSource == LaunchSource::OneTime)
            {
                PageIndication::OnceLaunchSwitchTrue();
                FreqMeter::LoadOneTime();
            }
            else if ((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsCountPulse() && PageModesA::modeMeasureCountPulse == ModeMeasureCountPulse::StartStop) ||
                (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsCountPulse() && PageModesB::modeMeasureCountPulse == ModeMeasureCountPulse::StartStop))
            {
                PageModesA::ToggleStartStop();
                FreqMeter::LoadStartStop();
            }
            else
            {
                if (Hint::Text()[0] != 0 && Hint::UnderItem() == this)
                {
                    NextChoice();
                }

                Hint::Create(this);

                return true;
            }
        }
    }
    return false;
}


void Switch::Draw(int x, int y, int width, bool selected)
{
    ColorDraw(selected).SetAsCurrent();

    Text(text).WriteInCenterRect(x, y, width, HEIGHT);
}


void Button::Draw(int x, int y, int width, bool selected)
{
    ColorDraw(selected).SetAsCurrent();

    Text(text).WriteInCenterRect(x, y, width, HEIGHT);
}


void Switch::CreateHint(char buffer[100]) const
{
    std::strcpy(buffer, hint);
    std::strcat(buffer, ": ");
    std::strcat(buffer, state->ToText());
}


Color Item::ColorBackground(bool selected)
{
    return selected ? Color::GRAY_75 : Color::GRAY_10;
}


Color Item::ColorDraw(bool selected)
{
    return selected ? Color::BLACK : Color::WHITE;
}


void Page::OnEvent(EventType::E event)
{
    if (onEvent)
    {
        onEvent(event);
    }
}
