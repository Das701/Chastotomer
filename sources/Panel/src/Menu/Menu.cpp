#include "defines.h"
#include "FreqMeter.h"
#include "Display/Display.h"
#include "Display/Font/Font.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Hint.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PagesSettings.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageModesA.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstring>

using namespace Display;

// Открывает страницу, соответствующую воздействию control. Возвращает false, если для воздействия нет соответствующей страницы
static bool OpenPage(Control control);

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

        if (!openedPage->OnControl(control))
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
            Math::CircleIncrease<uint8>((uint8*)&set.currentChannel, 0, Channel::Count - 1);
        }

        if (CURRENT_CHANNEL_IS_A)       { openedPage = PageSettingsA::self; }
        else if (CURRENT_CHANNEL_IS_B)  { openedPage = PageSettingsB::self; }
        else if (CURRENT_CHANNEL_IS_C)  { openedPage = PageSettingsC::self; }
        else if (CURRENT_CHANNEL_IS_D)  { openedPage = PageSettingsD::self; }

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
    
    Page *pageMode = nullptr;
    
    if (CURRENT_CHANNEL_IS_A)       { pageMode = PageModesA::self; }
    else if (CURRENT_CHANNEL_IS_B)  { pageMode = PageModesB::self; }
    else if (CURRENT_CHANNEL_IS_C)  { pageMode = PageModesC::self; }
    else if (CURRENT_CHANNEL_IS_D)  { pageMode = PageModesD::self; }
        
    Page * const pages[Control::Count] =
    {
        nullptr,                            //  0
        pageMode,                           //  Control::Mode
        PageIndication::self,               //  Control::Indication
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
    };

    SetCurrentChannel(control);

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
