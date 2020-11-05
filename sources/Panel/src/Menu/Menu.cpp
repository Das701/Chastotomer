#include "defines.h"
#include "FreqMeter.h"
#include "Display/Display.h"
#include "Display/Font/Font.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Hint.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageModesA.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstring>

using namespace Display;

// ��������� ��������, ��������������� ����������� control. ���������� false, ���� ��� ����������� ��� ��������������� ��������
static bool OpenPage(Control control);

// ������� ������������ �������� ����
static Page *openedPage = PageModesA::self;

// ������ �������
static bool firstPress = false;


void Menu::Init()
{
//    PageModesA::Init();
//    PageModesB::Init();
//    PageModesC::Init();
}


void Menu::Draw()
{
    openedPage->Draw(0, 216, 0);
}


bool Menu::Update()
{
    while (!Keyboard::Empty())
    {
        Display::Refresh();

        Control control = Keyboard::NextControl();

        if (!openedPage->OnControl(control))
        {
            OpenPage(control);
        }
        return true; //-V612
    }
    
    return false;
}


// ������������� ������� ����� � ����������� �� ������� ������
static void SetCurrentChannel(const Control &control)
{
    if (control.value == Control::Channels)
    {
        if(firstPress == false)
        {
            openedPage = PageChannelA::self;
            firstPress = true;
        }
        else
        {
            uint8 channel = (uint8)set.currentChannel;
            Math::CircleIncrease<uint8>(&channel, 0, Channel::Count - 1);
            set.currentChannel = (Channel::E)channel;
    
            Page *page = nullptr;
    
            if (CURRENT_CHANNEL_IS_A)
            {
                page = PageChannelA::self;
            }
            else if (CURRENT_CHANNEL_IS_B)
            {
                page = PageChannelB::self;
            }
            else if (CURRENT_CHANNEL_IS_C)
            {
                page = PageChannelC::self;
            }
            else if (CURRENT_CHANNEL_IS_D)
            {
                page = PageChannelD::self;
            }

            openedPage = page;
        }

        Hint::Hide();

        FreqMeter::LoadChannel();
    }
}


static bool OpenPage(Control control)
{
    if (!control.action.IsPress())
    {
        return false;
    }
    
    Page *pageMode = nullptr;
    
    if (CURRENT_CHANNEL_IS_A)
    {
        pageMode = PageModesA::self;
    } 
    else if (CURRENT_CHANNEL_IS_B)
    {
        pageMode = PageModesB::self;
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        pageMode = PageModesC::self;
    }
    else if (CURRENT_CHANNEL_IS_D)
    {
        pageMode = PageModesD::self;
    }       
        
    Page * const pages[Control::Count] =
    {
        /*  0 */  nullptr,
        /*  1 */  pageMode,
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


char *Menu::ChannelSettings()
{
// ��������� � ����� ������ settings ������ ugo ����� �������������� ������
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
        ADD_UGO(SU::Int2String(LEVEL_SYNCH_A * Divider::Current().ToAbs(), buffer));
        if (PageChannelA::typeSynch == TypeSynch::Manual)
        {
            std::strcat(settings, "��");
        }
        else
        {
            switch (PeriodTimeLabels::Current().value)
            {
            case PeriodTimeLabels::T_3:     std::strcat(settings, "x10-3");     break;
            case PeriodTimeLabels::T_4:     std::strcat(settings, "x10-4");     break;
            case PeriodTimeLabels::T_5:     std::strcat(settings, "x10-5");     break;
            case PeriodTimeLabels::T_6:     std::strcat(settings, "x10-6");     break;
            case PeriodTimeLabels::T_7:     std::strcat(settings, "x10-7");     break;
            case PeriodTimeLabels::T_8:     std::strcat(settings, "x10-8");     break;
            }
        }
    }
    
    if(CURRENT_CHANNEL_IS_B)
    {
        ADD_UGO(PageChannelB::couple.UGO());
        ADD_UGO(PageChannelB::impedance.UGO());
        ADD_UGO(PageChannelB::modeFilter.UGO());
        ADD_UGO(PageChannelB::modeFront.UGO());
        ADD_UGO(PageChannelB::divider.UGO());
        ADD_UGO(PageChannelB::typeSynch.UGO());
        ADD_UGO(SU::Int2String(LEVEL_SYNCH_B * Divider::Current().ToAbs(), buffer));
        if (PageChannelB::typeSynch == TypeSynch::Manual)
        {
            std::strcat(settings, "��");
        }
        else
        {
            switch (PeriodTimeLabels::Current().value)
            {
            case PeriodTimeLabels::T_3:     std::strcat(settings, "x10-3");     break;
            case PeriodTimeLabels::T_4:     std::strcat(settings, "x10-4");     break;
            case PeriodTimeLabels::T_5:     std::strcat(settings, "x10-5");     break;
            case PeriodTimeLabels::T_6:     std::strcat(settings, "x10-6");     break;
            case PeriodTimeLabels::T_7:     std::strcat(settings, "x10-7");     break;
            case PeriodTimeLabels::T_8:     std::strcat(settings, "x10-8");     break;
            }
        }
    }

    return settings;
}

Page* Menu::UsedPage()
{
    return openedPage;
}
