#pragma once
#include "Settings.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Menu/Pages/Channels/Channels.h"


extern Page pageModesA;
extern Page pageModesB;
extern Page pageModesC;
extern Page pageModesD;


struct CurrentPageModes
{
    // ���������� true, ���� ������� �������� ������� �������� ����� ���������
    static bool ConsistTimeMeasure();
};


// ����� ������� ����� ��� ���� ������� ������� �������
class PageModes
{
public:

    static void DrawParameters(int x, int y);
};


class PageModesA
{
public:
    static void RelationOn();
    static void RelationOff();
    static bool RelationCheck();

    static void PressSetup();
    static bool StartStop();
    static void ToggleStartStop();

    // ���������� ��� ��������� ������� ���������
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModePeriod();
    static void OnChanged_ModeDuration();
    static void OnChanged_ModeCountPulse();

};


struct PageModesB
{
    static void PressSetup();

    // ���������� ��� ��������� ������� ���������
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModePeriod();
    static void OnChanged_ModeDuration();
    static void OnChanged_ModeCountPulse();
};


struct PageModesC
{
    // ���������� ��� ��������� ������� ���������
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModeCountPulse();
};
