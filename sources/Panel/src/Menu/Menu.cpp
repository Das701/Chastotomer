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


// Открывает страницу, соответствующую воздействию control. Возвращает false, если для воздействия нет соответствующей страницы
static bool OpenPage(Control control);

// Обработка события кнопку
static void OnKey(const Control &control);

// Обработка события ручки
static void OnGovernor(const Control &control);

// Текущая отображаемая страница меню
static Page *openedPage = Channel::A->pageModes;

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
    if (PageIndication::calibration.IsPressed())
    {
        return;
    }

    if (CURRENT_CHANNEL_IS_A_OR_B && control.IsRotateGovernor())
    {
        int delta = Channel::Current()->set.typeSynch.IsHoldoff() ? 1 : 2;

        if (control.value == Control::GovLeft)
        {
            delta = -delta;
        }

        LevelSynch::Change(delta);
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


// Устанавливает текущий канал в зависимости от нажатой кнопки
static void SetCurrentChannel(const Control &control)
{
    if (control.value == Control::Channels)
    {
        bool loadToFPGA = false;
        
        if (Menu::OpenedPage()->IsPageSettings())
        {
            int num = Channel::Current()->Number();

            Math::CircleIncrease<uint8>((uint8 *)&num, 0, Channel::Count - 1);

            Channel::SetCurrent(num);

            loadToFPGA = true;
        }

        if (CURRENT_CHANNEL_IS_A)       { openedPage = Channel::A->pageSettings; }
        else if (CURRENT_CHANNEL_IS_B)  { openedPage = Channel::B->pageSettings; }
        else if (CURRENT_CHANNEL_IS_C)  { openedPage = Channel::C->pageSettings; }
        else if (CURRENT_CHANNEL_IS_D)  { openedPage = Channel::D->pageSettings; }

        Hint::Hide();

        if(loadToFPGA)
        {
            Channel::Current()->OnChanged_TypeMeasure();

            Channel::Current()->LoadToFPGA();
        }
    }

    if (control.value == Control::Mode)
    {
        if (CURRENT_CHANNEL_IS_A)      { openedPage = Channel::A->pageModes; }
        else if (CURRENT_CHANNEL_IS_B) { openedPage = Channel::B->pageModes; }
        else if (CURRENT_CHANNEL_IS_C) { openedPage = Channel::C->pageModes; }
        else if (CURRENT_CHANNEL_IS_D) { openedPage = Channel::D->pageModes; }

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

    std::strcpy(settings, names[NUMBER_CURRENT_CHANNEL]);

    if(CURRENT_CHANNEL_IS_A)
    {
        ADD_UGO(Channel::A->set.couple.UGO());
        ADD_UGO(Channel::A->set.impedance.UGO());
        ADD_UGO(Channel::A->set.modeFilter.UGO());
        ADD_UGO(Channel::A->set.modeFront.UGO());
        ADD_UGO(Channel::A->set.divider.UGO());
        ADD_UGO(Channel::A->set.typeSynch.UGO());
        ADD_UGO(SU::Int2String(LEVEL_SYNCH_A * Channel::Current()->set.divider.ToAbs()).c_str());
        if (Channel::A->set.typeSynch.IsManual())
        {
            std::strcat(settings, "мВ");
        }
        else
        {
            switch (Channel::timeLabels.value)
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
        ADD_UGO(Channel::B->set.couple.UGO());
        ADD_UGO(Channel::B->set.impedance.UGO());
        ADD_UGO(Channel::B->set.modeFilter.UGO());
        ADD_UGO(Channel::B->set.modeFront.UGO());
        ADD_UGO(Channel::B->set.divider.UGO());
        ADD_UGO(Channel::B->set.typeSynch.UGO());
        ADD_UGO(SU::Int2String(LEVEL_SYNCH_B * Channel::Current()->set.divider.ToAbs()).c_str());
        if (Channel::B->set.typeSynch.IsManual())
        {
            std::strcat(settings, "мВ");
        }
        else
        {
            switch (Channel::timeLabels.value)
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


static void OnKey(const Control &control) //-V2008
{
    switch (control.value)
    {
    case Control::GovButton:
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

    case Control::Enter:
        if (openedPage->SelectedItem())
        {
            openedPage->SelectedItem()->OnControl(control);
        }
        break;

    case Control::Mode:
        Channel::Current()->PressSetup();
        break;

    case Control::Test:
        if ((Channel::A->set.modeFrequency.IsRatioAC() && CURRENT_CHANNEL_IS_A) ||
            (Channel::B->set.modeFrequency.IsRatioBC() && CURRENT_CHANNEL_IS_B))
        {
        }
        else
        {
            FreqMeter::modeTest.Switch();
        }
        break;

    case Control::Auto:
        if ((Channel::A->set.typeMeasure.IsFrequency() && Channel::A->set.modeFrequency.IsFrequency() && CURRENT_CHANNEL_IS_A) ||
            (Channel::B->set.typeMeasure.IsFrequency() && Channel::B->set.modeFrequency.IsFrequency() && CURRENT_CHANNEL_IS_B) ||
            (Channel::C->set.typeMeasure.IsFrequency() && Channel::C->set.modeFrequency.IsFrequency() && CURRENT_CHANNEL_IS_C) ||
            (Channel::A->set.typeMeasure.IsPeriod() && Channel::A->set.modePeriod.IsPeriod() && CURRENT_CHANNEL_IS_A) ||
            (Channel::B->set.typeMeasure.IsPeriod() && Channel::B->set.modePeriod.IsPeriod() && CURRENT_CHANNEL_IS_B) ||
            (Channel::A->set.typeMeasure.IsDuration() && Channel::A->set.modeDuration.IsNdt() && CURRENT_CHANNEL_IS_A) ||
            (Channel::B->set.typeMeasure.IsDuration() && Channel::B->set.modeDuration.IsNdt() && CURRENT_CHANNEL_IS_B))
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
    FreqMeter::modeTest.AddObserver(Channel::A->pageModes);
    FreqMeter::modeTest.AddObserver(Channel::B->pageModes);
}
