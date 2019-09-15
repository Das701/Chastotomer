#include "defines.h"
#include "Menu.h"
#include "MenuItems.h"
#include "Menu/Pages/PageModes.h"
#include "Display/Text.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"
#include "Menu/Pages/PageChannelC.h"
#include "Menu/Pages/PageChannelD.h"
#include "Settings.h"
#include "Utils/String.h"
#include <cstring>


using namespace Display;

/// Открывает страницу, соответствующую воздействию control. Возвращает false, если для воздействия нет соответствующей страницы
static bool OpenPage(Control control);

/// Текущая отображаемая страница меню
static Page *openedPage = PageModes::self;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Init()
{
    PageModes::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Draw()
{
    font->SetType(Font::Type::_8);
 
    openedPage->Draw(35, 52);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Update()
{
    while (!Keyboard::Empty())
    {
        Control control = Keyboard::NextControl();
        
        if(!openedPage->OnControl(control))
        {
            OpenPage(control);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// Устанавливает текущий канал в зависимости от нажатой кнопки
static void SetCurrentChannel(const Control &control)
{
    if (control.value >= Control::A && control.value <= Control::D)
    {
        static const Channel::E chans[9] =
        {
            Channel::Count, Channel::Count, Channel::Count, Channel::Count, Channel::Count,
            Channel::A, Channel::B, Channel::C, Channel::D
        };

        set.currentChannel = chans[control.value];
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool OpenPage(Control control)
{
    if (!control.action.IsPress())
    {
        return false;
    }

    static Page * const pages[Control::Count] =
    {
/*  0 */  nullptr,
/*  1 */  PageModes::self,
/*  2 */  PageIndication::self,
/*  3 */  nullptr, nullptr,
/*  5 */  PageChannelA::self,
/*  6 */  PageChannelB::self,
/*  7 */  PageChannelC::self,
/*  8 */  PageChannelD::self,
/*  9 */  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
    };

    SetCurrentChannel(control);

    if (pages[control.value])
    {
        openedPage = pages[control.value];
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

    static char settings[100] = { 0 };
    if(set.currentChannel == Channel::A)
    {
        std::strcpy(settings, "A:");
        ADD_UGO(PageChannelA::channelInput.UGO());
        ADD_UGO(PageChannelA::inputImpedance.UGO());
        ADD_UGO(PageChannelA::modeFilter.UGO());
        ADD_UGO(PageChannelA::modeFront.UGO());
        ADD_UGO(PageChannelA::divider.UGO());
        ADD_UGO(PageChannelA::typeSynch.UGO());
        std::strcat(settings, Int2String(LEVEL_SYNCH_A, buffer));
    }
    else if(set.currentChannel == Channel::B)
    {
        std::strcpy(settings, "B:");
        ADD_UGO(PageChannelB::channelInput.UGO());
        ADD_UGO(PageChannelB::inputImpedance.UGO());
        ADD_UGO(PageChannelB::modeFilter.UGO());
        ADD_UGO(PageChannelB::modeFront.UGO());
        ADD_UGO(PageChannelB::divider.UGO());
        ADD_UGO(PageChannelB::typeSynch.UGO());
        std::strcat(settings, Int2String(LEVEL_SYNCH_B, buffer));
    }
    else if(set.currentChannel == Channel::C)
    {
        std::strcpy(settings, "C:");
    }
    else if(set.currentChannel == Channel::D)
    {
        std::strcpy(settings, "D:");
    }
    return settings;
}
