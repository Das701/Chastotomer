#include "defines.h"
#include "Menu.h"
#include "MenuItems.h"
#include "Menu/Pages/PageModes.h"
#include "Display/Text.h"


/// Горизонтальная координата верхнего левого угла меню
static int x0 = 36;
/// Вертикальная координата верхнего левого угла меню
static int y0 = 53;

static Page *page = PageModes::self;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Draw()
{
    font->SetType(Font::Type::_8);

    page->Draw(x0, y0);
}
