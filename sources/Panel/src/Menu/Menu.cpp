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
#include "Menu/Pages/PagesSettings.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageModesA.h"
#include "Menu/Pages/PageService.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstring>

using namespace Display;

// Открывает страницу, соответствующую воздействию control. Возвращает false, если для воздействия нет соответствующей страницы
static bool OpenPage(Control control);

// Обработка события клаиатуры
static bool OnControl(const Control &control);

// Текущая отображаемая страница меню
static Page *openedPage = PageModesA::self;


void Menu::Draw()
{
    openedPage->Draw(0, 216, 0);
}


bool Menu::Update()
{
    while (!Keyboard::Empty())
    {
        Display::Refresh();

        Control control = Keyboard::NextControl();

        bool processed = false;

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
                    processed = true;
                }
                else if (control.value == Control::GovRight)
                {
                    LevelSynch::Change(CURRENT_CHANNEL, 2);
                    FPGA::IncreaseN();
                    FPGA::WriteData();
                    processed = true;
                }
            }
        }

        if (!processed)
        {
            processed = OnControl(control);
        }

        if (!processed)
        {
            OpenPage(control);
        }
        return true; //-V612
    }
    
    return false;
}


// Устанавливает текущий канал в зависимости от нажатой кнопки
static void SetCurrentChannel(const Control &control)
{
    if (control.value == Control::Channels)
    {
        if (Menu::OpenedPage()->IsPageSettings())
        {
            Math::CircleIncrease<uint8>((uint8 *)&CURRENT_CHANNEL, 0, Channel::Count - 1);
        }

        if (CURRENT_CHANNEL_IS_A)       { openedPage = PageSettingsA::self; }
        else if (CURRENT_CHANNEL_IS_B)  { openedPage = PageSettingsB::self; }
        else if (CURRENT_CHANNEL_IS_C)  { openedPage = PageSettingsC::self; }
        else if (CURRENT_CHANNEL_IS_D)  { openedPage = PageSettingsD::self; }

        Hint::Hide();

        FreqMeter::LoadChannel();
    }

    if (control.value == Control::Mode)
    {
        if (Menu::OpenedPage()->IsPageModes())
        {
            Math::CircleIncrease<uint8>((uint8 *)&CURRENT_CHANNEL, 0, Channel::Count - 1);
        }

        if (CURRENT_CHANNEL_IS_A)      { openedPage = PageModesA::self; }
        else if (CURRENT_CHANNEL_IS_B) { openedPage = PageModesB::self; }
        else if (CURRENT_CHANNEL_IS_C) { openedPage = PageModesC::self; }
        else if (CURRENT_CHANNEL_IS_D) { openedPage = PageModesD::self; }

        Hint::Hide();

        FreqMeter::LoadChannel();
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


char *Menu::ChannelSettings()
{
// Добавляет в конец строки settings строку ugo через разделительный символ
#define ADD_UGO(ugo)            \
    std::strcat(settings, " "); \
    std::strcat(settings, ugo);

    char buffer[20];

    static const char * const names[Channel::Count] = { "A:", "B:", "C:", "D:" };

    static char settings[100] = { 0 };

    std::strcpy(settings, names[CURRENT_CHANNEL]);

    if(CURRENT_CHANNEL_IS_A)
    {
        ADD_UGO(PageSettingsA::couple.UGO());
        ADD_UGO(PageSettingsA::impedance.UGO());
        ADD_UGO(PageSettingsA::modeFilter.UGO());
        ADD_UGO(PageSettingsA::modeFront.UGO());
        ADD_UGO(PageSettingsA::divider.UGO());
        ADD_UGO(PageSettingsA::typeSynch.UGO());
        ADD_UGO(SU::Int2String(LEVEL_SYNCH_A * Divider::Current().ToAbs(), buffer));
        if (PageSettingsA::typeSynch == TypeSynch::Manual)
        {
            std::strcat(settings, "мВ");
        }
        else
        {
            switch (PeriodTimeLabels::Current().value)
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
        ADD_UGO(SU::Int2String(LEVEL_SYNCH_B * Divider::Current().ToAbs(), buffer));
        if (PageSettingsB::typeSynch == TypeSynch::Manual)
        {
            std::strcat(settings, "мВ");
        }
        else
        {
            switch (PeriodTimeLabels::Current().value)
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

    return settings;
}

Page* Menu::OpenedPage()
{
    return openedPage;
}


static bool OnControl(const Control &control) //-V2008
{
    bool result = false;

    switch (control.value)
    {
    case Control::Right:
        openedPage->SelectNextItem();
        Hint::Hide();
        result = true;
        break;

    case Control::Left:
        openedPage->SelectPrevItem();
        Hint::Hide();
        result = true;
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
            result = openedPage->SelectedItem()->OnControl(control);
        }
        break;

    case Control::Mode:
        if (CURRENT_CHANNEL_IS_A)
        {
            PageModesA::PressSetup();
        }
        else if (CURRENT_CHANNEL_IS_B)
        {
            PageModesB::PressSetupB();
        }
        break;

    case Control::Test:
        if ((PageModesA::modeMeasureFrequency.IsRatioAC() && CURRENT_CHANNEL_IS_A) ||
            (PageModesB::modeMeasureFrequency.IsRatioBC() && CURRENT_CHANNEL_IS_B))
        {
        }
        else
        {
            FreqMeter::LoadTest();
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
        break;

    default:
        // никаких действий по умолчанию производить не требуется
        break;
    }

    return result;
}
