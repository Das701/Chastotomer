#include "defines.h"
#include "Menu.h"
#include "MenuItems.h"
#include "Menu/Pages/PageModes.h"
#include "Display/Display.h"
#include "Display/Text.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"
#include "Menu/Pages/PageChannelC.h"
#include "Menu/Pages/PageChannelD.h"
#include "Menu/Hint.h"
#include "Settings.h"
#include "Utils/String.h"
#include "Utils/Math.h"
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

    openedPage->Draw(0, 210);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Update()
{
    while (!Keyboard::Empty())
    {
        Control control = Keyboard::NextControl();

        if (!openedPage->OnControl(control))
        {
            OpenPage(control);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// Устанавливает текущий канал в зависимости от нажатой кнопки
static void SetCurrentChannel(const Control &control)
{
    if (control.value == Control::Channels)
    {
        uint8 channel = (uint8)set.currentChannel;
        Math::CircleIncrease<uint8>(&channel, 0, Channel::Count - 1);
        set.currentChannel = (Channel::E)channel;

        Page *page = nullptr;

        if (set.currentChannel == Channel::A)
        {
            page = PageChannelA::self;
        }
        else if (set.currentChannel == Channel::B)
        {
            page = PageChannelB::self;
        }
        else if (set.currentChannel == Channel::C)
        {
            page = PageChannelC::self;
        }
        else if (set.currentChannel == Channel::D)
        {
            page = PageChannelD::self;
        }

        openedPage = page;
        Hint::Hide();
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
        /*  3 */  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
    };

    SetCurrentChannel(control);

    Page *page = pages[control.value];

    if (page && page != openedPage)
    {
        openedPage = page;
        Hint::Hide();
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

    static const char * const names[Channel::Count] = { "A:", "B:", "C:", "D:" };

    static char settings[100] = { 0 };

    std::strcpy(settings, names[CURRENT_CHANNEL]);

    if(CURRENT_CHANNEL_IS_A)
    {
        ADD_UGO(PageChannelA::couple.UGO());
        ADD_UGO(PageChannelA::impedance.UGO());
        ADD_UGO(PageChannelA::modeFilter.UGO());
        ADD_UGO(PageChannelA::modeFront.UGO());
        ADD_UGO(PageChannelA::divider.UGO());
        ADD_UGO(PageChannelA::typeSynch.UGO());
        ADD_UGO(Int2String(LEVEL_SYNCH_A, buffer));
        std::strcat(settings, "мВ");
    }
    
    if(CURRENT_CHANNEL_IS_B)
    {
        ADD_UGO(PageChannelB::couple.UGO());
        ADD_UGO(PageChannelB::impedance.UGO());
        ADD_UGO(PageChannelB::modeFilter.UGO());
        ADD_UGO(PageChannelB::modeFront.UGO());
        ADD_UGO(PageChannelB::divider.UGO());
        ADD_UGO(PageChannelB::typeSynch.UGO());
        ADD_UGO(Int2String(LEVEL_SYNCH_B, buffer));
        std::strcat(settings, "мВ");
    }

    return settings;
}
