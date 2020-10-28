#include "defines.h"
#include "Menu.h"
#include "MenuItems.h"
#include "Menu/Pages/PageModesA.h"
#include "Menu/Pages/PageModesB.h"
#include "Menu/Pages/PageModesC.h"
#include "Menu/Pages/PageModesD.h"
#include "Display/Display.h"
#include "Display/Text.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"
#include "Menu/Pages/PageChannelC.h"
#include "Menu/Pages/PageChannelD.h"
#include "Menu/Hint.h"
#include "Settings.h"
#include "Utils/Math.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include <cstring>
#include "FreqMeter/FreqMeter.h"

using namespace Display;

/// Открывает страницу, соответствующую воздействию control. Возвращает false, если для воздействия нет соответствующей страницы
static bool OpenPage(Control control);

/// Текущая отображаемая страница меню
static Page *openedPage = PageModesA::self;

/// Первое нажатие
static bool firstPress = false;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Init()
{
//    PageModesA::Init();
//    PageModesB::Init();
//    PageModesC::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Draw()
{
    font->SetType(Font::Type::_8);

    openedPage->Draw(0, 210);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::Update()
{
    while (!Keyboard::Empty())
    {
        Control control = Keyboard::NextControl();

        if (!openedPage->OnControl(control))
        {
            OpenPage(control);
        }
        return true;
    }
    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// Устанавливает текущий канал в зависимости от нажатой кнопки
static void SetCurrentChannel(const Control &control)
{
    if (control.value == Control::Channels)
    {
        if(firstPress == false)
        {
            openedPage = PageChannelA::self;
            firstPress = true;
        }
        else
        {
            uint8 channel = (uint8)set.currentChannel;
            Math::CircleIncrease<uint8>(&channel, 0, Channel::Count - 1);
            set.currentChannel = (Channel::E)channel;
    
            Page *page = nullptr;
    
            if (CURRENT_CHANNEL_IS_A)
            {
                page = PageChannelA::self;
            }
            else if (CURRENT_CHANNEL_IS_B)
            {
                page = PageChannelB::self;
            }
            else if (CURRENT_CHANNEL_IS_C)
            {
                page = PageChannelC::self;
            }
            else if (set.currentChannel == Channel::D)
            {
                page = PageChannelD::self;
            }
            openedPage = page;
        }
        Hint::Hide();
        FreqMeter::LoadChannel();
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool OpenPage(Control control)
{
    if (!control.action.IsPress())
    {
        return false;
    }
    
    Page *pageMode = nullptr;
    
    if (CURRENT_CHANNEL_IS_A)
    {
        pageMode = PageModesA::self;
    } 
    else if (CURRENT_CHANNEL_IS_B)
    {
        pageMode = PageModesB::self;
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        pageMode = PageModesC::self;
    }
    else if (CURRENT_CHANNEL_IS_D)
    {
        pageMode = PageModesD::self;
    }       
        
    Page * const pages[Control::Count] =
    {
        /*  0 */  nullptr,
        /*  1 */  pageMode,
        /*  2 */  PageIndication::self,
        /*  3 */  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *Menu::ChannelSettings()
{
/// Добавляет в конец строки settings строку ugo через разделительный символ
#define ADD_UGO(ugo)            \
    std::strcat(settings, " "); \
    std::strcat(settings, ugo);

    char buffer[20];

    static const char * const names[Channel::Count] = { "A:", "B:", "C:", "D:" };

    static char settings[100] = { 0 };

    std::strcpy(settings, names[CURRENT_CHANNEL]);

    if(CURRENT_CHANNEL_IS_A)
    {
        ADD_UGO(PageChannelA::couple.UGO());
        ADD_UGO(PageChannelA::impedance.UGO());
        ADD_UGO(PageChannelA::modeFilter.UGO());
        ADD_UGO(PageChannelA::modeFront.UGO());
        ADD_UGO(PageChannelA::divider.UGO());
        ADD_UGO(PageChannelA::typeSynch.UGO());
        ADD_UGO(SU::Int2String(LEVEL_SYNCH_A, buffer));
        if (PageChannelA::typeSynch == TypeSynch::Manual)
        {
            std::strcat(settings, "мВ");
        }
        else
        {
            if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_7 && CURRENT_CHANNEL_IS_A)||
                (PageModesB::periodTimeLabels == PeriodTimeLabels::T_7 && CURRENT_CHANNEL_IS_B)||
                (PageModesC::periodTimeLabels == PeriodTimeLabels::T_7 && CURRENT_CHANNEL_IS_C))
                {
                    std::strcat(settings, "x10-7");
                }
                else if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_6 && CURRENT_CHANNEL_IS_A)||
                (PageModesB::periodTimeLabels == PeriodTimeLabels::T_6 && CURRENT_CHANNEL_IS_B)||
                (PageModesC::periodTimeLabels == PeriodTimeLabels::T_6 && CURRENT_CHANNEL_IS_C))
                {
                    std::strcat(settings, "x10-6");
                }
                else if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_5 && CURRENT_CHANNEL_IS_A)||
                (PageModesB::periodTimeLabels == PeriodTimeLabels::T_5 && CURRENT_CHANNEL_IS_B)||
                (PageModesC::periodTimeLabels == PeriodTimeLabels::T_5 && CURRENT_CHANNEL_IS_C))
                {
                    std::strcat(settings, "x10-5");
                }
                else if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_4 && CURRENT_CHANNEL_IS_A)||
                (PageModesB::periodTimeLabels == PeriodTimeLabels::T_4 && CURRENT_CHANNEL_IS_B)||
                (PageModesC::periodTimeLabels == PeriodTimeLabels::T_4 && CURRENT_CHANNEL_IS_C))
                {
                    std::strcat(settings, "x10-4");
                }
                else if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_3 && CURRENT_CHANNEL_IS_A)||
                (PageModesB::periodTimeLabels == PeriodTimeLabels::T_3 && CURRENT_CHANNEL_IS_B)||
                (PageModesC::periodTimeLabels == PeriodTimeLabels::T_3 && CURRENT_CHANNEL_IS_C))
                {
                    std::strcat(settings, "x10-3");
                }
                else if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_8 && CURRENT_CHANNEL_IS_A)||
                (PageModesB::periodTimeLabels == PeriodTimeLabels::T_8 && CURRENT_CHANNEL_IS_B)||
                (PageModesC::periodTimeLabels == PeriodTimeLabels::T_8 && CURRENT_CHANNEL_IS_C))
                {
                    std::strcat(settings, "x10-8");
                }
        }
    }
    
    if(CURRENT_CHANNEL_IS_B)
    {
        ADD_UGO(PageChannelB::couple.UGO());
        ADD_UGO(PageChannelB::impedance.UGO());
        ADD_UGO(PageChannelB::modeFilter.UGO());
        ADD_UGO(PageChannelB::modeFront.UGO());
        ADD_UGO(PageChannelB::divider.UGO());
        ADD_UGO(PageChannelB::typeSynch.UGO());
        ADD_UGO(SU::Int2String(LEVEL_SYNCH_B, buffer));
        if (PageChannelB::typeSynch == TypeSynch::Manual)
        {
            std::strcat(settings, "мВ");
        }
        else
        {
            if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_7 && CURRENT_CHANNEL_IS_A)||
                (PageModesB::periodTimeLabels == PeriodTimeLabels::T_7 && CURRENT_CHANNEL_IS_B)||
                (PageModesC::periodTimeLabels == PeriodTimeLabels::T_7 && CURRENT_CHANNEL_IS_C))
                {
                    std::strcat(settings, "x10-7");
                }
                else if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_6 && CURRENT_CHANNEL_IS_A)||
                (PageModesB::periodTimeLabels == PeriodTimeLabels::T_6 && CURRENT_CHANNEL_IS_B)||
                (PageModesC::periodTimeLabels == PeriodTimeLabels::T_6 && CURRENT_CHANNEL_IS_C))
                {
                    std::strcat(settings, "x10-6");
                }
                else if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_5 && CURRENT_CHANNEL_IS_A)||
                (PageModesB::periodTimeLabels == PeriodTimeLabels::T_5 && CURRENT_CHANNEL_IS_B)||
                (PageModesC::periodTimeLabels == PeriodTimeLabels::T_5 && CURRENT_CHANNEL_IS_C))
                {
                    std::strcat(settings, "x10-5");
                }
                else if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_4 && CURRENT_CHANNEL_IS_A)||
                (PageModesB::periodTimeLabels == PeriodTimeLabels::T_4 && CURRENT_CHANNEL_IS_B)||
                (PageModesC::periodTimeLabels == PeriodTimeLabels::T_4 && CURRENT_CHANNEL_IS_C))
                {
                    std::strcat(settings, "x10-4");
                }
                else if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_3 && CURRENT_CHANNEL_IS_A)||
                (PageModesB::periodTimeLabels == PeriodTimeLabels::T_3 && CURRENT_CHANNEL_IS_B)||
                (PageModesC::periodTimeLabels == PeriodTimeLabels::T_3 && CURRENT_CHANNEL_IS_C))
                {
                    std::strcat(settings, "x10-3");
                }
                else if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_8 && CURRENT_CHANNEL_IS_A)||
                (PageModesB::periodTimeLabels == PeriodTimeLabels::T_8 && CURRENT_CHANNEL_IS_B)||
                (PageModesC::periodTimeLabels == PeriodTimeLabels::T_8 && CURRENT_CHANNEL_IS_C))
                {
                    std::strcat(settings, "x10-8");
                }
        }
    }

    return settings;
}

static Page* Menu::UsedPage()
{
    return openedPage;
}
