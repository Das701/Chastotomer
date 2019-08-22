#include "defines.h"
#include "Menu.h"
#include "MenuItems.h"
#include "Menu/Pages/PageModes.h"
#include "Display/Text.h"


/// �������������� ���������� �������� ������ ���� ����
static int x0 = 36;
/// ������������ ���������� �������� ������ ���� ����
static int y0 = 53;

static Page *page = PageModes::self;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Draw()
{
    font->SetType(Font::Type::_8);

    page->Draw(x0, y0);
}
