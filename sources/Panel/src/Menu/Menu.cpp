#include "defines.h"
#include "Menu.h"
#include "MenuItems.h"
#include "Menu/Pages/PageModes.h"
#include "Display/Text.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Pages/PageModeIndication.h"
#include "Menu/Pages/PageModeChannels.h"
#include "Menu/Pages/PageModeChannelsB.h"

using namespace Display;


/// √оризонтальна€ координата верхнего левого угла меню
static int x0 = 36;
/// ¬ертикальна€ координата верхнего левого угла меню
static int y0 = 53;

static Page *openedPage = PageModes::self;

/// ѕоследний нажатый элемент управлени€
//static Control lastControl;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
        
         if(control.value == Control::Mode) 
        {
            openedPage = PageModes::self;
        }
        if(control.value == Control::Indication) 
        {
            openedPage = PageModeIndication::self;
        }
        if(control.value == Control::_A) 
        {
            openedPage = PageModeChannels::self;
        }
        if(control.value == Control::_B) 
        {
            openedPage = PageModeChannelsB::self;
        }
        if(control.value == Control::_C) 
        {
            openedPage = &Page::empty;
        }
        if(control.value == Control::_D) 
        {
            openedPage = &Page::empty;
        }
    }
}
