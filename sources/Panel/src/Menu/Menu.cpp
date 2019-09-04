#include "defines.h"
#include "Menu.h"
#include "MenuItems.h"
#include "Menu/Pages/PageModes.h"
#include "Display/Text.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"
#include <cstring>


using namespace Display;

/// Открывает страницу, соответствующую воздействию control. Возвращает false, если для воздействия нет соответствующей страницы
static bool OpenPage(Control control);

/// Горизонтальная координата верхнего левого угла меню
static int x0 = 36;
/// Вертикальная координата верхнего левого угла меню
static int y0 = 53;
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
 
    openedPage->Draw(x0, y0);
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

    switch (control.value)
    {
    case Control::Mode:
        openedPage = PageModes::self;
        return true;

    case Control::Indication:
        openedPage = PageIndication::self;
        return true;

    case Control::A:
        openedPage = PageChannelA::self;
        return true;

    case Control::B:
        openedPage = PageChannelB::self;
        return true;

    case Control::C:
        openedPage = &Page::emptyC;
        return true;
    
    case Control::D:
        openedPage = &Page::emptyD;
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *Menu::Hint()
{
    return openedPage->hint;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void DefaultSettings(char *settings)
{
    std::strcat(settings, "ОВ ");
    std::strcat(settings, "1МОм");
    std::strcat(settings, " ");
    std::strcat(settings, "Фронт");
    std::strcat(settings, " ");
    std::strcat(settings, "0mV ");        
}


char *Menu::ChannelSettings()
{
    static char settings[100] = { 0 };
    if(openedPage == PageChannelA::self)
    {
        std::strcpy(settings, "A: ");
        std::strcat(settings, PageChannelA::channelInput.UGO());
        std::strcat(settings, PageChannelA::inputImpedance.UGO());
        std::strcat(settings, PageChannelA::modeFilter.UGO());
        std::strcat(settings, PageChannelA::modeFront.UGO());
        std::strcat(settings, PageChannelA::divider.UGO());
        std::strcat(settings, PageChannelA::levelSynch.UGO());        
    }
    else if(openedPage == PageChannelB::self)
    {
        std::strcpy(settings, "B: ");
        std::strcat(settings, PageChannelA::channelInput.UGO());
        std::strcat(settings, PageChannelB::inputImpedance.UGO());
        std::strcat(settings, PageChannelB::modeFilter.UGO());
        std::strcat(settings, PageChannelB::modeFront.UGO());
        std::strcat(settings, PageChannelB::divider.UGO());
        std::strcat(settings, PageChannelB::levelSynch.UGO()); 
    }
    else if(openedPage == &Page::emptyC)
    {
        std::strcpy(settings, "C: ");
        DefaultSettings(settings);
    }
    else if(openedPage == &Page::emptyD)
    {
        std::strcpy(settings, "D: ");
        DefaultSettings(settings);
    }
    else
    {
       std::strcpy(settings, "A: ");
       DefaultSettings(settings); 
    }
    return settings;
}
