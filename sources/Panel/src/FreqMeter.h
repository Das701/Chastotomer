#pragma once
#include "Menu/MenuItems.h"


// ������� ��� �������� ����� "����"
struct ModeTest : public Enumeration
{
    enum E
    {
        Disabled,
        Enabled
    };

    explicit ModeTest(E v) : Enumeration((uint8)v) {};

    static bool IsEnabled();
};


namespace FreqMeter
{
    // �������� ������ ��������� �������
    void LoadModeMeasureFrequency();

    // �������� ������ ��������� �������
    void LoadModeMeasurePeriod();

    // �������� ������ ��������� ������������
    void LoadModeMeasureDuration();

    // �������� ������ ��������� ����� ���������
    void LoadModeMeasureCountPulse();

    // �������� ������� ����� �������
    void LoadPeriodTimeLabels();

    // �������� ������� ���������
    void LoadTimeMeasure();

    // �������� ����� �������� ���������
    void LoadNumerPeriodsMeasure();

    // �������� �������� ������������� �� �������� ������
    void LoadImpedance();

    void LoadModeFront();

    void LoadDivider();

    void LoadTypeSynch();
    
    void LoadChannel();
    
    void LoadMemoryMode();
    
    void LoadCalibration();
    
    void LoadLaunchSource();
    
    void LoadRefGenerator();
    
    void LoadDisplayTime();
    
    // ����������� ����� "����"
    void SwitchModeTest();
    
    void LoadAuto();
    
    void LoadMeasure();
    
    void UnloadAuto();
    
    void LoadOneTime();
    
    void LoadStartStop();
}
