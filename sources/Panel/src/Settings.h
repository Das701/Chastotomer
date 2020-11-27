#pragma once
#include "Hardware/FreqMeter.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Menu/Pages/Channels/Channels.h"


#define CURRENT_CHANNEL             (Channel::current)
#define CURRENT_CHANNEL_IS_A        (CURRENT_CHANNEL == Channel::A)
#define CURRENT_CHANNEL_IS_B        (CURRENT_CHANNEL == Channel::B)
#define CURRENT_CHANNEL_IS_A_OR_B   (CURRENT_CHANNEL_IS_A || CURRENT_CHANNEL_IS_B)
#define CURRENT_CHANNEL_IS_C        (CURRENT_CHANNEL == Channel::C)
#define CURRENT_CHANNEL_IS_D        (CURRENT_CHANNEL == Channel::D)

#define TYPE_SYNCH(ch)              (set.typeSynch[(ch)])
#define TYPE_SYNCH_A                (TYPE_SYNCH(Channel::A))
#define TYPE_SYNCH_B                (TYPE_SYNCH(Channel::B))

#define LEVEL_SYNCH(ch)             (set.levelSynch[ch][TYPE_SYNCH(ch)])
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
