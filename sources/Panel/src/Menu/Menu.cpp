#include "defines.h"
#include "Settings.h"
#include "Display/Display.h"
#include "Display/Font/Font.h"
#include "Keyboard/Keyboard.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Hint.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageService.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstring>


// ќткрывает страницу, соответствующую воздействию control. ¬озвращает false, если дл€ воздействи€ нет соответствующей страницы
static bool OpenPage(Control control);

// ќбработка событи€ кнопку
static void OnKey(const Control &control);

// ќбработка событи€ ручки
static void OnGovernor(const Control &control);

// “екуща€ отображаема€ страница меню
static Page *openedPage = nullptr;

static void SubscribeToEvents();


void Menu::Draw()
{
    openedPage->Draw(0, Display::PHYSICAL_HEIGHT - Item::HEIGHT - 1, 0);

    if (openedPage == PageStatistics::self)
    {
        PageStatistics::Draw();
    }
}


static void OnGovernor(const Control &control)
{
    if (CURRENT_CHANNEL_IS_A_OR_B && control.IsRotateGovernor())
    {
        int delta = Channel::Current()->set.typeSynch.IsHoldoff() ? 1 : 2;

        if (control.value == Control::GovLeft)
        {
            delta = -delta;
        }

        if (PageIndication::calibration.Is(Calibration::Pressed))
        {
            if (control.value == Control::GovLeft)
            {
                FPGA::DecreaseN();
            }
            else
            {
                FPGA::IncreaseN();
            }
            FPGA::WriteDataGovernor();
        }
        else
        {
            LevelSynch::Change(delta);
        }
    }
}


void Menu::Update()
{
    while (!Keyboard::Empty())
    {
        Control control = Keyboard::NextControl();

        OnGovernor(control);

        OnKey(control);

        OpenPage(control);

        Display::Refresh();
    }
}


// ”станавливает текущий канал в зависимости от нажатой кнопки
static void SetCurrentChannel(const Control &control)
{
    if (control.value == Control::Channels)
    {
        if (Menu::OpenedPage()->IsPageSettings())
        {
            Channel::SelectNext();

            Channel::Current()->OnChanged_TypeMeasure();

            Channel::Current()->LoadToFPGA();
        }

        openedPage = Channel::Current()->pageSettings;

        Hint::Hide();
    }
    else if (control.value == Control::Mode)
    {
        openedPage = Channel::Current()->pageModes;

        Hint::Hide();
    }
}


static bool OpenPage(Control control) //-V2506
{
    if (!control.action.IsPress())
    {
        return false;
    }
           
    SetCurrentChannel(control);

    Page *const pages[Control::Count] =
    {
        nullptr,              // GovButton,
        nullptr,              // Mode,
        PageIndication::self, // Indication,
        nullptr,              // Left,
        nullptr,              // Right,
        nullptr,              // Channels,
        nullptr,              // Enter,
        PageService::self,    // Service,
        nullptr,              // GovLeft,
        nullptr,              // GovRight,
        nullptr,              // Test,
        nullptr,              // Auto,
        nullptr               // None,
    };

    Page *page = pages[control.value];

    if (page && page != openedPage)
    {
        openedPage = page;
        Hint::Hide();
        return true;
    }

    return false;
}


Page* Menu::OpenedPage()
{
    return openedPage;
}


void Menu::SetOpenedPage(Page *page)
{
    openedPage = page;
}


static void OnKey(const Control &control) //-V2008
{
    switch (control.value)
    {
    case Control::GovButton:
    case Control::Enter:
        if (openedPage->SelectedItem())
        {
            openedPage->SelectedItem()->OnControl(control);
        }
        break;

    case Control::Right:
        openedPage->SelectNextItem();
        break;

    case Control::Left:
        openedPage->SelectPrevItem();
        break;

    case Control::Mode:
        Channel::Current()->PressSetup();
        break;

    case Control::Test:
        if ((Channel::A->mod.modeFrequency.IsRatioAC() && CURRENT_CHANNEL_IS_A) ||
            (Channel::B->mod.modeFrequency.IsRatioBC() && CURRENT_CHANNEL_IS_B))
        {
        }
        else
        {
            FreqMeter::modeTest.Switch();
        }
        break;

    case Control::Auto:
        if ((Channel::A->mod.typeMeasure.IsFrequency() && Channel::A->mod.modeFrequency.IsFrequency() && CURRENT_CHANNEL_IS_A) ||
            (Channel::B->mod.typeMeasure.IsFrequency() && Channel::B->mod.modeFrequency.IsFrequency() && CURRENT_CHANNEL_IS_B) ||
            (Channel::C->mod.typeMeasure.IsFrequency() && Channel::C->mod.modeFrequency.IsFrequency() && CURRENT_CHANNEL_IS_C) ||
            (Channel::A->mod.typeMeasure.IsPeriod() && Channel::A->mod.modePeriod.IsPeriod() && CURRENT_CHANNEL_IS_A) ||
            (Channel::B->mod.typeMeasure.IsPeriod() && Channel::B->mod.modePeriod.IsPeriod() && CURRENT_CHANNEL_IS_B) ||
            (Channel::A->mod.typeMeasure.IsDuration() && Channel::A->mod.modeDuration.IsNdt() && CURRENT_CHANNEL_IS_A) ||
            (Channel::B->mod.typeMeasure.IsDuration() && Channel::B->mod.modeDuration.IsNdt() && CURRENT_CHANNEL_IS_B))
        {
            MathFPGA::Auto::Refresh();
            FreqMeter::LoadAuto();
            FPGA::SwitchAuto();
        }
        break;

    case Control::Indication:
    case Control::Channels:
    case Control::Service:
    case Control::Count:
    case Control::GovLeft:
    case Control::GovRight:
    case Control::None:
    default:
        // никаких действий по умолчанию производить не требуетс€
        break;
    }
}


void Menu::Init()
{
    openedPage = Channel::A->pageModes;

    SubscribeToEvents();
}


static void SubscribeToEvents()
{
    FreqMeter::modeTest.AddObserver(Channel::A->pageModes);
    FreqMeter::modeTest.AddObserver(Channel::B->pageModes);
}
