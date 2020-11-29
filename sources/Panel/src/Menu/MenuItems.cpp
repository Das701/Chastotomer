#include "defines.h"
#include "Settings.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/FPGA.h"
#include "Hardware/FreqMeter.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Hint.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Menu/Pages/PageIndication.h"
#include "Utils/Math.h"
#include <cstring>


using namespace Primitives;


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


String Enumeration::ToString() const
{
    return String(names[IndexName()]);
}


int Enumeration::IndexName() const
{
    if (correct == nullptr)
    {
        return value;
    }

    int index = 0;

    for (int i = 0; i < value; i++)
    {
        if (correct[i])
        {
            index++;
        }
    }

    return index;
}


char *Enumeration::UGO() const
{
    return ugo[IndexName()];
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
        return (Display::PHYSICAL_WIDTH / 4);
    }

    int result = ((num % 2) == 0) ? ((Display::PHYSICAL_WIDTH - 1) / 6) : (Display::PHYSICAL_WIDTH / 6);

    if (NumItems() == 5)
    {
        result = ((num % 2) == 0) ? ((Display::PHYSICAL_WIDTH - 1) / 5) : (Display::PHYSICAL_WIDTH / 5);

        if (num == 4) { result += 3; }

        return result;
    }

    if (NumItems() == 4)
    {
        result = ((num % 2) == 0) ? ((Display::PHYSICAL_WIDTH - 1) / 4) : (Display::PHYSICAL_WIDTH / 4);

        if (num == 3) { result += 2; }

        return result;
    }

    if (num == 5) { result += 3; }

    return result;
}


bool Page::IsPageModes() const
{
    return (this == Channel::A.pageModes) || (this == Channel::B.pageModes) || (this == Channel::C.pageModes) || (this == Channel::D.pageModes);
}


bool Page::IsPageSettings() const
{
    return (this == Channel::A.pageSettings) || (this == Channel::B.pageSettings) || (this == Channel::C.pageSettings) || (this == Channel::D.pageSettings);
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
    Hint::Create(SelectedItem());
}


void Page::SelectPrevItem()
{
    Math::CircleDecrease<int>(&selectedItem, 0, NumItems() - 1);
    Hint::Create(SelectedItem());
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
            else if ((CURRENT_CHANNEL_IS_A && Channel::A.set.typeMeasure.IsCountPulse() && Channel::A.modeMeasureCountPulse.Is_StartStop()) ||
                (CURRENT_CHANNEL_IS_B && Channel::B.set.typeMeasure.IsCountPulse() && Channel::B.modeMeasureCountPulse.Is_StartStop()))
            {
                ModeStartStop::Toggle();
                FreqMeter::LoadStartStop();
            }
            else
            {
                NextChoice();

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


void Switch::CreateHint(String &h) const
{
    h.Free();
    h.Append(hint);
    h.Append(": ");
    h.Append(state->ToString().c_str());
}


void Button::CreateHint(String &h) const
{
    h.Free();
    h.Append(hint);
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


TypeMeasure *Page::GetTypeMeasure() const
{
    Switch *item = (Switch *)items[0];

    return (TypeMeasure *)item->state;
}


int Page::GetModeMeasure() const
{
    Switch *item = (Switch *)items[1];

    Enumeration *state = item->state;

    return state->value;
}


bool Page::ExistTypeMeasure(uint8 type) const
{
    Switch *item = (Switch *)items[0];

    return item->state->correct[type];
}


void Page::ResetTypeAndModeMeasure()
{
    Switch *type = (Switch *)items[0];

    if (type->state->correct == nullptr)
    {
        type->state->value = 0;
    }
    else
    {
        for (int i = 0; i < type->state->numStates; i++)
        {
            if (type->state->correct[i])
            {
                type->state->value = (uint8)i;
                break;
            }
        }
    }

    ResetModeMeasure();
}


bool Page::ExistModeMeasure(int mode) const
{
    Switch *item = (Switch *)items[1];

    return item->state->correct[mode];
}

void Page::ResetModeMeasure()
{
    Switch *mode = (Switch *)items[1];

    if (mode->state->correct == nullptr)
    {
        mode->state->value = 0;
    }
    else
    {
        for (int i = 0; i < mode->state->numStates; i++)
        {
            if (mode->state->correct[i])
            {
                mode->state->value = (uint8)i;
                break;
            }
        }
    }
}


//void Page::SetTypeAndModeMeasure(int t, int m)
//{
//    Switch *type = (Switch *)items[0];
//
//    type->state->value = (uint8)t;
//
//    Switch *mode = (Switch *)items[1];
//
//    mode->state->value = (uint8)m;
//
//    if (this == Channel::A.pageModes)
//    {
//        PageModesA::OnChanged_TypeMeasure();
//    }
//    else if (this == Channel::B.pageModes)
//    {
//        PageModesB::OnChanged_TypeMeasure();
//    }
//    else if (this == Channel::C.pageModes)
//    {
//        PageModesC::OnChanged_TypeMeasure();
//    }
//    else if (this == Channel::D.pageModes)
//    {
//    }
//}
