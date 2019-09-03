#include "defines.h"
#include "PageChannelA.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include <cstring>

using namespace Display::Primitives;
using namespace Display;

extern Item *items[];


ChannelInput    PageChannelA::channelInput(ChannelInput::Closed);
InputImpedance  PageChannelA::inputImpedance(InputImpedance::_1MOmh);
ModeFilter      PageChannelA::modeFilter(ModeFilter::Off);
ModeFront       PageChannelA::modeFront(ModeFront::Front);
Divider         PageChannelA::divider(Divider::_1);
LevelSynch      PageChannelA::levelSynch(LevelSynch::TTL);


static const char *settings[] = { "A: ", "ОВ ", "1МОм", " ", "Фронт", " ", "0mV ", nullptr };

void Switch::CreateChannelSettings()
{

   channelSettings[0] = 0;
        for (int i = 0; settings[i] != 0; i++)
        {
            
            std::strcat(channelSettings, settings[i]);
        }

}
int count = 0;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_OCI()
{
    
    if(count == 0)
    {
        settings[1] = "ЗВ ";
        count++;
    }
    else
    {
        settings[1] = "ОВ ";
        count--;
    }
    Switch::CreateChannelSettings();
}

static char *namesInput[] = { "Откр. вход", "Закр. вход", nullptr };

/// Выбор закрытого(открытого) входа текущего канала
static Switch sInputChoice(
    "ОЗВ", "Вход", namesInput,
    &PageChannelA::channelInput,
    &OnPress_OCI
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Impedance()
{
//    int count = 0;
//    if(count == 0)
//    {
//        settings[2] = "50Ом";
//        count++;
//    }
//    else
//    {
//        settings[2] = "1МОм";
//        count--;
//    }
//    Switch::CreateChannelSettings();
}

static char *namesImpedance[] = { "1 МОм", "50 Ом", nullptr };

/// Установка входного сопротивления текущего канала
static Switch sImpedance(
    "Rвх", "Входное сопротивление текущего канала", namesImpedance,
    &PageChannelA::inputImpedance,
    &OnPress_Impedance
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Filter()
{
//
//    if(count == 0)
//    {
//        settings[3] = " ФНЧ ";
//        count++;
//    }
//    else
//    {
//        settings[3] = " ";
//        count--;
//    }
//    Switch::CreateChannelSettings();
}

static char *namesLowpassFilter[] = { "Откл.", "Вкл.", nullptr };

/// Включение(отключение) ФНЧ
static Switch sLowpassFilter(
    "ФНЧ", "Включение/отключение фильтра НЧ", namesLowpassFilter,
    &PageChannelA::modeFilter,
    &OnPress_Filter
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Front()
{
//    int count = 0;
//    if(count == 0)
//    {
//        settings[4] = "Срез";
//        count++;
//    }
//    else
//    {
//        settings[4] = "Фронт";
//        count--;
//    }
//    Switch::CreateChannelSettings();
}

static char *namesFront[] = { "Фронт", "Срез", nullptr };

/// Выбор фронта синхронизации текущего канала
static Switch sFront(
    "Фронт", "Выбор типа синхронизации", namesFront,
    &PageChannelA::modeFront,
    &OnPress_Front
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_VD()
{
//    int count = 0;
//    if(count == 0)
//    {
//        settings[5] = " 1/10 ";
//        count++;
//    }
//    else
//    {
//        settings[5] = " ";
//        count--;
//    }
//    Switch::CreateChannelSettings();
}

static char *namesDivider[] = { "1:1", "1:10", nullptr };

/// Включение(отключение) входного делителя напряжения
static Switch sDivider(
    "1/1 1/10", "Включение/отключение входного делителя напряжения", namesDivider,
    &PageChannelA::divider,
    &OnPress_VD
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Sync()
{
//    int count = 0;
//    if(count == 0)
//    {
//        settings[6] = "1200mV";
//        count++;
//    }
//    else
//    {
//        settings[6] = "-1240mV";
//        count--;
//    }
//    Switch::CreateChannelSettings();
}

static char *namesSync[] = { "ТТЛ", "ЭСЛ", nullptr };

/// Выбор уровня синхронизации ТТЛ, ЭСЛ
static Switch sSync(
    "Синхр", "Выбор уровня сихронизации", namesSync,
    &PageChannelA::levelSynch,
    &OnPress_Sync
);

static Item *items[] =
{
    &sInputChoice,
    &sImpedance,
    &sLowpassFilter,
    &sFront,
    &sDivider,
    &sSync,
    nullptr
};

static Page pageChannelA(items);

Page *PageChannelA::self = &pageChannelA;
