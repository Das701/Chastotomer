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

/// ќткрывает страницу, соответствующую воздействию control. ¬озвращает false, если дл€ воздействи€ нет соответствующей страницы
static bool OpenPage(Control control);

/// “екуща€ отображаема€ страница меню
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
/*  3 */  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
/* 10 */  PageChannelA::self,
/* 11 */  PageChannelB::self,
/* 12 */  PageChannelC::self,
/* 13 */  PageChannelD::self,
/* 14 */  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
    };

    if (pages[control.value])
    {
        openedPage = pages[control.value];
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *Menu::Hint()
{
    return openedPage->Item::Hint();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *Menu::ChannelSettings()
{
/// ƒобавл€ет в конец строки settings строку ugo через разделительный символ
#define ADD_UGO(ugo)            \
    std::strcat(settings, " "); \
    std::strcat(settings, ugo);

    static char settings[100] = { 0 };
    if(openedPage == PageChannelA::self)
    {
        std::strcpy(settings, "A:");
        ADD_UGO(PageChannelA::channelInput.UGO());
        ADD_UGO(PageChannelA::inputImpedance.UGO());
        ADD_UGO(PageChannelA::modeFilter.UGO());
        ADD_UGO(PageChannelA::modeFront.UGO());
        ADD_UGO(PageChannelA::divider.UGO());
        ADD_UGO(PageChannelA::levelSynch.UGO());
        char buffer[20];
        std::strcat(settings, Int2String(set.syncValue, buffer));
    }
    else if(openedPage == PageChannelB::self)
    {
        std::strcpy(settings, "B:");
        ADD_UGO(PageChannelA::channelInput.UGO());
        ADD_UGO(PageChannelB::inputImpedance.UGO());
        ADD_UGO(PageChannelB::modeFilter.UGO());
        ADD_UGO(PageChannelB::modeFront.UGO());
        ADD_UGO(PageChannelB::divider.UGO());
        ADD_UGO(PageChannelB::levelSynch.UGO());
    }
    else if(openedPage == PageChannelC::self)
    {
        std::strcpy(settings, "C:");
    }
    else if(openedPage == PageChannelD::self)
    {
        std::strcpy(settings, "D:");
    }
    return settings;
}
