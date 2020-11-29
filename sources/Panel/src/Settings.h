#pragma once
#include "Hardware/FreqMeter.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Menu/Pages/Channels/Channels.h"


#define TYPE_SYNCH(ch)              (set.typeSynch[NUMBER_CHANNEL(ch)])
#define TYPE_SYNCH_A                (TYPE_SYNCH(Channel::A))
#define TYPE_SYNCH_B                (TYPE_SYNCH(Channel::B))

#define LEVEL_SYNCH(ch)             (set.levelSynch[NUMBER_CHANNEL(ch)][TYPE_SYNCH(NUMBER_CHANNEL(ch))])
#define LEVEL_SYNCH_A               (LEVEL_SYNCH(Channel::A))
#define LEVEL_SYNCH_B               (LEVEL_SYNCH(Channel::B))


struct LevelSynch
{
    // �������� ������� ������������� �� �������� ���������� ���������� (�������� �� �����������. �� ����������� ������ ��� ������ �� �������)
    static void Change(int delta);
};


struct Settings
{
    int          levelSynch[4][2];  // ������� �������������
    TypeSynch::E typeSynch[4];      // ��� ������������� ��� ������� �� �������
    bool         showStatistics;    // ���������� �� ���������� �� FPS
};

extern Settings set;
