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
#include "Menu/Pages/Pages.h"
#include "Menu/Pages/PageIndication.h"
#include "Utils/Math.h"
#include <cstring>


using namespace Display::Primitives;
using namespace Display;

int info = 0;



int Enumeration::NumStates() const
{
    int result = 0;

    for (int i = 0; names[i] != nullptr; i++)
    {
        result++;
    }

    return result;
}


bool Button::OnControl(const Control &)
{
    return true;
}


void Button::Draw(int x, int y, int width, bool selected)
{
    if (selected)
    {
        Color::BLACK.SetAsCurrent();
    }
    Text(text).Write(x, y + 2, width);
}



void Page::Draw(int x, int y, int, bool)
{
    for (int i = 0; i < NumItems(); i++)
    {
        if (i == selectedItem)
        {
            Rectangle(WidthItem(i), HEIGHT - 1).Fill(x + 1, y + 1, Color::WHITE);
        }
        items[i]->Draw(x, y, WidthItem(i), i == selectedItem);
        Rectangle(WidthItem(i), HEIGHT).Draw(x, y, Color::WHITE);

        x += WidthItem(i);
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
        result += 2;
    }

    return result;
}


bool Page::IsPageModes() const
{
    return this == PageModesA::self || this == PageModesB::self || this == PageModesC::self || this == PageModesD::self;
}


static bool ChangeLevelSynch(int delta)
{
    LevelSynch::Change(CURRENT_CHANNEL, delta);

    return true;
}


bool Page::OnControl(const Control &control) //-V2008
{
    bool result = false;

    switch (control.value)
    {
    case Control::Right:
        if (PageIndication::calibration.Is(Calibration::Pressed))
        {
            if (SelectedItem())
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
        if (PageIndication::calibration.Is(Calibration::Pressed))
        {
            if (SelectedItem())
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

        if (PageIndication::calibration.Is(Calibration::Pressed))
        {
        }
        else if(CURRENT_CHANNEL_IS_A_OR_B)
        {
            result = ChangeLevelSynch(-2);
        }

        FPGA::DecreaseN();
        FPGA::WriteData();
        info = 3;
        break;

    case Control::GovRight:

        if (PageIndication::calibration.Is(Calibration::Pressed))
        {
        }
        else if(CURRENT_CHANNEL_IS_A_OR_B)
        {
            result = ChangeLevelSynch(2);
        }

        FPGA::IncreaseN();
        FPGA::WriteData();
        info = 4;
        break;

    case Control::Enter:
        if (SelectedItem())
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
        if (PageIndication::calibration.Is(Calibration::Pressed))
        {
            if (SelectedItem())
            {
                result = SelectedItem()->OnControl(control);
            }
        }
        if (CURRENT_CHANNEL_IS_A)
        {
            PageModesA::PressSetup();
        }
        else if (CURRENT_CHANNEL_IS_B)
        {
            PageModesB::PressSetupB();
        }
        info = 8;
        break;
    case Control::Indication:
        if (PageIndication::calibration.Is(Calibration::Pressed))
        {
            if (SelectedItem())
            {
                result = SelectedItem()->OnControl(control);
            }
        }
        info = 9;
        break;
    case Control::Channels:
        if (PageIndication::calibration.Is(Calibration::Pressed))
        {
            if (SelectedItem())
            {
                result = SelectedItem()->OnControl(control);
            }
        }
        info = 10;
        break;
    case Control::GovButton:
        if (SelectedItem())
        {
            result = SelectedItem()->OnControl(control);
        }
        info = 11;
        break;
    case Control::Esc:
        if (PageIndication::calibration.Is(Calibration::Pressed))
        {
            if (SelectedItem())
            {
                result = SelectedItem()->OnControl(control);
            }
        }
        else
        {
            PageModesA::RelationOff();
            PageModesA::InterpolateOff();
            PageModesA::DCycleOff();
        }
        info = 12;
        break;
    case Control::Test:
        if (PageIndication::calibration.Is(Calibration::Pressed))
        {
            if (SelectedItem())
            {
                result = SelectedItem()->OnControl(control);
            }
        }
        else
        {
            if ((PageModesA::modeMeasureFrequency.IsRatioAC() && CURRENT_CHANNEL_IS_A) ||
                (PageModesB::modeMeasureFrequency.IsRatioBC() && CURRENT_CHANNEL_IS_B))
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
        if (PageIndication::calibration.Is(Calibration::Pressed))
        {
            if (SelectedItem())
            {
                result = SelectedItem()->OnControl(control);
            }
        }
        else
        {
            if ((PageModesA::typeMeasure.IsFrequency() && PageModesA::modeMeasureFrequency.IsFrequency() && CURRENT_CHANNEL_IS_A) ||
                (PageModesB::typeMeasure.IsFrequency() && PageModesB::modeMeasureFrequency.IsFrequency() && CURRENT_CHANNEL_IS_B) ||
                (PageModesC::typeMeasure.IsFrequency() && PageModesC::modeMeasureFrequency.IsFrequency() && CURRENT_CHANNEL_IS_C) ||
                (PageModesA::typeMeasure.IsPeriod() && PageModesA::modeMeasurePeriod.IsPeriod() && CURRENT_CHANNEL_IS_A) ||
                (PageModesB::typeMeasure.IsPeriod() && PageModesB::modeMeasurePeriod.IsPeriod() && CURRENT_CHANNEL_IS_B) ||
                (PageModesA::typeMeasure.IsDuration() && PageModesA::modeMeasureDuration.Is_Ndt() && CURRENT_CHANNEL_IS_A) ||
                (PageModesB::typeMeasure.IsDuration() && PageModesB::modeMeasureDuration.Is_Ndt() && CURRENT_CHANNEL_IS_B))
            {
                MathFPGA::Auto::Refresh();
                FreqMeter::LoadAuto();
                FPGA::SwitchAuto();
            }
        }
        info = 14;
        break;

    default:
        // никаких действий по умолчанию производить не требуется
        break;
    }

    return result;
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



bool Switch::OnControl(const Control &control)
{
    if (PageIndication::calibration.Is(Calibration::Pressed))
    {
        if (control.action.IsPress() && (control.value != Control::GovRight || control.value != Control::GovLeft)) //-V560
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
        if (control.action.IsPress() && (control.value == Control::GovButton))
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

        if (control.action.IsPress() && (control.value == Control::Enter))
        {

            if (PageIndication::launchSource == LaunchSource::OneTime)
            {
                PageIndication::OnceLaunchSwitchTrue();
                FreqMeter::LoadOneTime();
            }
            else if ((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsCountPulse() && PageModesA::modeMeasureCountPulse == ModeMeasureCountPulseA::StartStop) ||
                (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsCountPulse() && PageModesB::modeMeasureCountPulse == ModeMeasureCountPulseB::StartStop))
            {
                PageModesA::ToggleStartStop();
                FreqMeter::LoadStartStop();
            }
            else
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
    }
    return false;
}


void Switch::Draw(int x, int y, int width, bool selected)
{
    if (selected)
    {
        Color::BLACK.SetAsCurrent();
    }

    Text(text).WriteInCenterRect(x, y, width, HEIGHT);
}


void Switch::CreateHint(char buffer[100]) const
{
    std::strcpy(buffer, hint);
    std::strcat(buffer, ": ");
    std::strcat(buffer, state->ToText());
}

