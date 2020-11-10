#pragma once


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
