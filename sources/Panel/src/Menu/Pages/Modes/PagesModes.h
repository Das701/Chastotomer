#pragma once
#include "Settings.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Menu/Pages/Channels/Channels.h"


struct CurrentPageModes
{
    // ���������� true, ���� ������� �������� ������� �������� ����� ���������
    static bool ConsistTimeMeasure();
};


// ����� ������� ����� ��� ���� ������� ������� �������
class PageModes
{
public:

    static void OnChanged_TypeMeasure();

    static void DrawParameters(int x, int y);

    static Page *Current();
};


class PageModesA
{
public:

    static Page *self;
    
    static void RelationOn();
    static void RelationOff();
    static bool RelationCheck();

    static void PressSetup();
    static bool StartStop();
    static void ToggleStartStop();

    // ���������� ��� ���������� ���� ���������
    static void OnChanged_TypeMeasure();

    // ���������� ��� ��������� ������� ���������
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModePeriod();
    static void OnChanged_ModeDuration();
    static void OnChanged_ModeCountPulse();

    static TypeMeasure           typeMeasure;                // ��� ���������
    static ModeMeasureFrequency  modeMeasureFrequency;       // ����� ��������� �������
    static ModeMeasurePeriod     modeMeasurePeriod;          // ����� ��������� �������
    static ModeMeasureDuration   modeMeasureDuration;        // ����� ��������� ������������
    static ModeMeasureCountPulse modeMeasureCountPulse;      // ����� ����� ���������
};


struct PageModesB
{
    static Page *self;

    static void PressSetup();

    // ���������� ��� ���������� ���� ���������
    static void OnChanged_TypeMeasure();

    // ���������� ��� ��������� ������� ���������
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModePeriod();
    static void OnChanged_ModeDuration();
    static void OnChanged_ModeCountPulse();

    static TypeMeasure           typeMeasure;           // ��� ���������
    static ModeMeasureFrequency  modeMeasureFrequency;  // ����� ��������� �������
    static ModeMeasurePeriod     modeMeasurePeriod;     // ����� ��������� �������
    static ModeMeasureDuration   modeMeasureDuration;   // ����� ��������� ������������
    static ModeMeasureCountPulse modeMeasureCountPulse; // ����� ����� ���������
};


struct PageModesC
{
    static Page *self;

    // ���������� ��� ���������� ���� ���������
    static void OnChanged_TypeMeasure();

    // ���������� ��� ��������� ������� ���������
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModeCountPulse();

    static TypeMeasure           typeMeasure;           // ��� ���������
    static ModeMeasureFrequency  modeMeasureFrequency;  // ����� ��������� �������
    static ModeMeasureCountPulse modeMeasureCountPulse; // ����� ����� ���������
};



class PageModesD
{
public:
    static Page *self;

    static TypeMeasure          typeMeasure;
    static ModeMeasureFrequency modeMeasureFrequency;
};
