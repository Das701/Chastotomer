#include "defines.h"
#include "Menu.h"
#include "MenuItems.h"
#include "Menu/Pages/PageModes.h"
#include "Display/Text.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"


using namespace Display;

/// ќткрывает страницу, соответствующую воздействию control. ¬озвращает false, если дл€ воздействи€ нет соответствующей страницы
static bool OpenPage(Control control);

/// √оризонтальна€ координата верхнего левого угла меню
static int x0 = 36;
/// ¬ертикальна€ координата верхнего левого угла меню
static int y0 = 53;
/// “екуща€ отображаема€ страница меню
static Page *openedPage = PageModes::self;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Init()
{
    PageModes::Init();

    
    Switch::CreateChannelSettings();
    
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
    case Control::D:
        openedPage = &Page::empty;
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *Menu::Hint()
{
    return openedPage->hint;
}

