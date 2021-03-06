#include "defines.h"
#include "Settings.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageService.h"
#include "Menu/Pages/Modes/Modes.h"


static void OnPress_Information()
{

}

DEF_BUTTON(bInformation, "����", "����� ��������� ����������", OnPress_Information);


static void OnPress_ResetSettings()
{
    TimeMeasure::Set(TimeMeasure::_1ms);
    NumberPeriods::Set(NumberPeriods::_1);
    PeriodTimeLabels::Set(PeriodTimeLabels::T_8);

    Channel::A->mod.typeMeasure.value = TypeMeasure::Frequency;
    Channel::A->mod.modeFrequency.value = ModeFrequency::Frequency;
    
    if(FreqMeter::modeTest.IsEnabled())
    {
        FreqMeter::modeTest.Switch();
    }
    LevelSynch::Set(0);
    
    Channel::SetCurrent(Channel::A);

    Menu::SetOpenedPage(Channel::A->pageModes);
    
    Channel::Current()->OnChanged_TypeMeasure(); //-V760

    Channel::Current()->LoadToFPGA();

    Channel::Current()->OnChanged_TypeMeasure();

    Channel::Current()->LoadToFPGA();
    
    LevelSynch::Set(0);
    
    FreqMeter::HardwareReset();
}

DEF_BUTTON(bResetSettings, "�����", "����� �������� � ��������� �� ���������", OnPress_ResetSettings);


static void OnPress_Language()
{

}

DEF_BUTTON(bLanguage, "����", "����� ����� ����������", OnPress_Language);


static void OnPress_ShowStatistics()
{
    gset.showStatistics = !gset.showStatistics;
}

DEF_BUTTON(bShowStatistics, "����������", "���������� ��������� ����������", OnPress_ShowStatistics);


static void OnPress_StyleGUI()
{
    gset.styleGUI.Switch();
}


DEF_BUTTON(bStyleGUI, "���������", "����� ���������� ������������", OnPress_StyleGUI);


static Item *items[7] =
{
    &bInformation,
    &bResetSettings,
    &bLanguage,
    &bStyleGUI,
    &bShowStatistics,
    nullptr,
    nullptr
};

void PageService::UseReset()
{
    OnPress_ResetSettings();
}

static Page pageService(items, nullptr);

Page *PageService::self = &pageService;
