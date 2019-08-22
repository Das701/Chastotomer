#include "defines.h"
#include "Menu.h"
#include "MenuItems.h"
#include "Menu/Pages/PageModes.h"
#include "Display/Text.h"
#include "Keyboard/Keyboard.h"


using namespace Display;


/// �������������� ���������� �������� ������ ���� ����
static int x0 = 36;
/// ������������ ���������� �������� ������ ���� ����
static int y0 = 53;

static Page *page = PageModes::self;
/// ��������� ������� ������� ����������
static Control lastControl;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Draw()
{
    font->SetType(Font::Type::_8);

    page->Draw(x0, y0);

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
