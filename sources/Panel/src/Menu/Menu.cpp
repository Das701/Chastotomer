#include "defines.h"
#include "FreqMeter.h"
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
#include "Menu/Pages/Modes/PagesModes.h"
#include "Menu/Pages/Settings/PagesSettings.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstring>


// Открывает страницу, соответствующую воздействию control. Возвращает false, если для воздействия нет соответствующей страницы
static bool OpenPage(Control control);

// Обработка события клаиатуры
static void OnControl(const Control &control);

// Текущая отображаемая страница меню
static Page *openedPage = PageModesA::self;

static void SubscribeToEvents();


void Menu::Draw()
{
    openedPage->Draw(0, Display::HEIGHT - Item::HEIGHT - 1, 0);

    if (openedPage == PageStatistics::self)
    {
        PageStatistics::Draw();
    }
}


bool Menu::Update()
{
    while (!Keyboard::Empty())
    {
        Control control = Keyboard::NextControl();

        if (PageIndication::calibration.IsPressed())
        {
        }
        else
        {
            if (CURRENT_CHANNEL_IS_A_OR_B)
            {
                if (control.value == Control::GovLeft)
                {
                    LevelSynch::Change(CURRENT_CHANNEL, -2);
                    FPGA::DecreaseN();
                    FPGA::WriteData();
                }
                else if (control.value == Control::GovRight)
                {
                    LevelSynch::Change(CURRENT_CHANNEL, 2);
                    FPGA::IncreaseN();
                    FPGA::WriteData();
                }
            }
        }

        OnControl(control);

        OpenPage(control);

        Display::Refresh();

        return true; //-V612
    }
    
    return false;
}


// Устанавливает текущий канал в зависимости от нажатой кнопки
static void SetCurrentChannel(const Control &control)
{
    if (control.value == Control::Channels)
    {
        bool loadToFPGA = false;
        
        if (Menu::OpenedPage()->IsPageSettings())
        {
            Math::CircleIncrease<uint8>((uint8 *)&CURRENT_CHANNEL, 0, Channel::Count - 1);
            loadToFPGA = true;
        }

        if (CURRENT_CHANNEL_IS_A)       { openedPage = PageSettingsA::self; }
        else if (CURRENT_CHANNEL_IS_B)  { openedPage = PageSettingsB::self; }
        else if (CURRENT_CHANNEL_IS_C)  { openedPage = PageSettingsC::self; }
        else if (CURRENT_CHANNEL_IS_D)  { openedPage = PageSettingsD::self; }

        Hint::Hide();

        if(loadToFPGA)
        {
            PageModes::OnChanged_TypeMeasure();

            HAL_TIM::DelayMS(10);

            Channel::LoadCurrentToFPGA();
        }
    }

    if (control.value == Control::Mode)
    {
        if (CURRENT_CHANNEL_IS_A)      { openedPage = PageModesA::self; }
        else if (CURRENT_CHANNEL_IS_B) { openedPage = PageModesB::self; }
        else if (CURRENT_CHANNEL_IS_C) { openedPage = PageModesC::self; }
        else if (CURRENT_CHANNEL_IS_D) { openedPage = PageModesD::self; }

        Hint::Hide();
    }
}


static bool OpenPage(Control control)
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


String Menu::ChannelSettings()
{
// Добавляет в конец строки settings строку ugo через разделительный символ
#define ADD_UGO(ugo)            \
    std::strcat(settings, " "); \
    std::strcat(settings, ugo);

    static const char * const names[Channel::Count] = { "Канал A:", "Канал B:", "Канал C:", "Канал D:" };

    char settings[100] = { 0 };

    std::strcpy(settings, names[CURRENT_CHANNEL]);

    if(CURRENT_CHANNEL_IS_A)
    {
        ADD_UGO(PageSettingsA::couple.UGO());
        ADD_UGO(PageSettingsA::impedance.UGO());
        ADD_UGO(PageSettingsA::modeFilter.UGO());
        ADD_UGO(PageSettingsA::modeFront.UGO());
        ADD_UGO(PageSettingsA::divider.UGO());
        ADD_UGO(PageSettingsA::typeSynch.UGO());
        ADD_UGO(SU::Int2String(LEVEL_SYNCH_A * Divider::Current().ToAbs()).c_str());
        if (PageSettingsA::typeSynch == TypeSynch::Manual)
        {
            std::strcat(settings, "мВ");
        }
        else
        {
            switch (PageModes::timeLabels.value)
            {
            case PeriodTimeLabels::T_3:     std::strcat(settings, "x10-3");     break;
            case PeriodTimeLabels::T_4:     std::strcat(settings, "x10-4");     break;
            case PeriodTimeLabels::T_5:     std::strcat(settings, "x10-5");     break;
            case PeriodTimeLabels::T_6:     std::strcat(settings, "x10-6");     break;
            case PeriodTimeLabels::T_7:     std::strcat(settings, "x10-7");     break;
            case PeriodTimeLabels::T_8:     std::strcat(settings, "x10-8");     break;
            }
        }
    }
    
    if(CURRENT_CHANNEL_IS_B)
    {
        ADD_UGO(PageSettingsB::couple.UGO());
        ADD_UGO(PageSettingsB::impedance.UGO());
        ADD_UGO(PageSettingsB::modeFilter.UGO());
        ADD_UGO(PageSettingsB::modeFront.UGO());
        ADD_UGO(PageSettingsB::divider.UGO());
        ADD_UGO(PageSettingsB::typeSynch.UGO());
        ADD_UGO(SU::Int2String(LEVEL_SYNCH_B * Divider::Current().ToAbs()).c_str());
        if (PageSettingsB::typeSynch == TypeSynch::Manual)
        {
            std::strcat(settings, "мВ");
        }
        else
        {
            switch (PageModes::timeLabels.value)
            {
            case PeriodTimeLabels::T_3:     std::strcat(settings, "x10-3");     break;
            case PeriodTimeLabels::T_4:     std::strcat(settings, "x10-4");     break;
            case PeriodTimeLabels::T_5:     std::strcat(settings, "x10-5");     break;
            case PeriodTimeLabels::T_6:     std::strcat(settings, "x10-6");     break;
            case PeriodTimeLabels::T_7:     std::strcat(settings, "x10-7");     break;
            case PeriodTimeLabels::T_8:     std::strcat(settings, "x10-8");     break;
            }
        }
    }

    return String(settings);
}

Page* Menu::OpenedPage()
{
    return openedPage;
}


void Menu::SetOpenedPage(Page *page)
{
    openedPage = page;
}


static void OnControl(const Control &control) //-V2008
{
    switch (control.value)
    {
    case Control::Right:
        openedPage->SelectNextItem();
        break;

    case Control::Left:
        openedPage->SelectPrevItem();
        break;

    case Control::GovLeft:
        FPGA::DecreaseN();
        FPGA::WriteData();
        break;

    case Control::GovRight:
        FPGA::IncreaseN();
        FPGA::WriteData();
        break;

    case Control::GovButton:
        if (openedPage->SelectedItem())
        {
            openedPage->SelectedItem()->OnControl(control);
        }
        break;

    case Control::Enter:
        if (openedPage->SelectedItem())
        {
            openedPage->SelectedItem()->OnControl(control);
        }
        break;

    case Control::Mode:
        if (CURRENT_CHANNEL_IS_A)
        {
            PageModesA::PressSetup();
        }
        else if (CURRENT_CHANNEL_IS_B)
        {
            PageModesB::PressSetup();
        }
        break;

    case Control::Test:
        if ((PageModesA::modeMeasureFrequency.IsRatioAC() && CURRENT_CHANNEL_IS_A) ||
            (PageModesB::modeMeasureFrequency.IsRatioBC() && CURRENT_CHANNEL_IS_B))
        {
        }
        else
        {
            FreqMeter::modeTest.Switch();
        }
        break;

    case Control::Auto:
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
        break;

    case Control::Indication:
    case Control::Channels:
    case Control::Service:
    case Control::Count:
    case Control::None:
    default:
        // никаких действий по умолчанию производить не требуется
        break;
    }
}


void Menu::Init()
{
    SubscribeToEvents();
}


static void SubscribeToEvents()
{
    FreqMeter::modeTest.AddObserver(PageModesA::self);
    FreqMeter::modeTest.AddObserver(PageModesB::self);
}
