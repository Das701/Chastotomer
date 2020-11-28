#include "defines.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Hardware/MathFPGA.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Modes/PagesModes.h"


using namespace Primitives;


extern Item *items[7];
extern Switch sModeFrequency;
extern Switch sModeCountPulse;


DEF_SWITCH_2(sTypeMeasure,
    "���������", "����� ���������",
    "�������", "��. ���.",
    Channel::C.typeMeasure, Channel::OnChanged_TypeMeasure
)


void Channel::OnChanged_ModeFrequencyC()
{
    items[1] = &sModeFrequency;

    switch (Channel::C.modeFrequency.value)
    {
    case ModeFrequency::Frequency:
        items[2] = Channel::switchTimeMeasue;
        items[3] = nullptr;
        break;

    case ModeFrequency::RatioCA:
    case ModeFrequency::RatioCB:
        items[2] = Channel::switchNumberPeriods;
        items[3] = Channel::switchTimeLabels;
        items[4] = nullptr;
        break;
    }

    PageModesA::RelationOff();
    
    ModeFrequency::LoadToFPGA();
}

// ����� ������ ��������� �������, ��������� ������, "���������"
DEF_SWITCH_3(sModeFrequency,
    "�����", "��������� �������",
    "�������", "f(C)/f(A)", "f(C)/f(B)",
    Channel::C.modeFrequency, Channel::OnChanged_ModeFrequencyC
);


void PageModesC::OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulse;

    switch (Channel::C.modeMeasureCountPulse.value)
    {
    case ModeCountPulse::CtA:
    case ModeCountPulse::CtB:
        items[2] = nullptr;
        break;

    case ModeCountPulse::CTA:
    case ModeCountPulse::CTB:
        items[2] = Channel::switchNumberPeriods;
        break;
    }

    items[3] = nullptr;

    PageModesA::RelationOff();
    
    ModeCountPulse::LoadToFPGA();
}

// ����� ������ ����� ���������
DEF_SWITCH_4(sModeCountPulse,
    "�����", "���� ����� ���������",
    "C(tA)", "C(tB)", "C(TA)", "C(TB)",
    Channel::C.modeMeasureCountPulse, PageModesC::OnChanged_ModeCountPulse
);


static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    Channel::switchTimeMeasue,
    nullptr
};

Page pageModesC(items, nullptr);
