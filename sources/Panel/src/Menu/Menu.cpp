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

static Page *page = PageModes::self;
static Page *page1 = PageModeIndication::self;
static Page *page2 = PageModeChannels::self;
static Page *page3 = PageModeChannelsB::self;
static Page empty;
/// ѕоследний нажатый элемент управлени€
static Control lastControl;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Draw()
{
    font->SetType(Font::Type::_8);
 
    if (lastControl.value == Control::Indication)
    {
        page1->Draw(x0, y0);
    }
    if (lastControl.value == Control::Mode)
    {
        page->Draw(x0, y0);   
    }
    if (lastControl.value == Control::_A)
    {
        page2->Draw(x0, y0);   
    }
    if (lastControl.value == Control::_B)
    {
        page3->Draw(x0, y0);   
    }
    if (lastControl.value == Control::_C)
    {
       Page empty;
    }
    
    Text(lastControl.Name()).Write(10, 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Update()
{
    while (!Keyboard::Empty())
    {
        lastControl = Keyboard::NextControl();
    }
}
