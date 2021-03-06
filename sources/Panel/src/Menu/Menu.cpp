#include "defines.h"
#include "Settings.h"
#include "Calculate/ValueFPGA.h"
#include "Display/Display.h"
#include "Display/Font/Font.h"
#include "Keyboard/Keyboard.h"
#include "Hardware/FPGA.h"
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


// ��������� ��������, ��������������� ����������� control. ���������� false, ���� ��� ����������� ��� ��������������� ��������
static bool OpenPage(Control control);

// ��������� ������� ������
static bool OnKey(const Control &control);

// ��������� ������� �����
static void OnGovernor(const Control &control);

// ������� ������������ �������� ����
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

        if (PageIndication::calibrationMode.IsEnabled())
        {
            if (control.value == Control::GovLeft)
            {
                FPGA::GovernorData::DecreaseN();
            }
            else
            {
                FPGA::GovernorData::IncreaseN();
            }
            
            FPGA::GovernorData::Write();
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

        if (!OnKey(control))
        {
            OpenPage(control);
        }

        Display::Refresh();
    }
}





static bool OpenPage(Control control)
{
    if (!control.action.IsPress())
    {
        return false;
    }
           
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


static bool OnKey(const Control &control) //-V2008
{
    if (PageIndication::calibrationMode.IsEnabled() &&
        control.value != Control::GovButton &&
        control.value != Control::GovLeft &&
        control.value != Control::GovRight)
    {
        FPGA::GovernorData::Reset();

        PageIndication::calibrationMode.value = CalibrationMode::Disabled;

        FreqMeter::LoadCalibrationNoSave();
        
        return true;
    }

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
        openedPage = (openedPage == Channel::Current()->pageModes) ? Channel::Current()->pageSettings : 
                                                                     Channel::Current()->pageModes;
        Hint::Hide();
        return true;

    case Control::Channels:
        {
            bool openSettings = (Menu::OpenedPage() == Channel::Current()->pageSettings);
            bool openModes = (Menu::OpenedPage() == Channel::Current()->pageModes);
            Channel::SetCurrentNext();
            Channel::LoadCurrentToFPGA(openSettings, openModes);
        }
        return true;

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
            FPGA::Auto::Refresh();
            FreqMeter::LoadAuto();
            FPGA::SwitchAuto();
        }
        break;

    case Control::Indication:
    case Control::Service:
    case Control::Count:
    case Control::GovLeft:
    case Control::GovRight:
    case Control::None:
    default:
        // ������� �������� �� ��������� ����������� �� ���������
        break;
    }

    return false;
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
