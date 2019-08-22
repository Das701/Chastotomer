#include "defines.h"
#include "Menu.h"
#include "MenuItems.h"
#include "Menu/Pages/PageModes.h"
#include "Display/Text.h"


/// �������������� ���������� �������� ������ ���� ����
static int x0 = 36;
/// ������������ ���������� �������� ������ ���� ����
static int y0 = 54;

static Page *page = PageModes::self;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Draw()
{
    page->Draw(x0, y0);

    Display::Text("Test string. �������� ������. 123").Write(10, 10, Color::WHITE);
}
