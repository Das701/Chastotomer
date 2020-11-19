#pragma once
#include "FreqMeter.h"
#include "Menu/MenuItems.h"


#define CURRENT_CHANNEL             (set.currentChannel)
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


struct Channel
{
    enum E
    {
        A,
        B,
        C,
        D,
        Count
    };

    // ��������� ������� ����� � ����������
    static void LoadCurrentToFPGA();
};


struct LevelSynch
{
    static const int MIN = -800;
    static const int MAX = 800;

    // �������� ������� ������������� �� �������� ���������� ���������� (�������� �� �����������. �� ����������� ������ ��� ������ �� �������)
    static void Change(Channel::E ch, int delta);
    // ���������� ������� ������� ������������� � ������������
    static int Current();
};


// ��������/�������� ����
struct InputCouple : public Enumeration
{
    enum E
    {
        AC,         // ��������
        DC          // ��������
    };

    explicit InputCouple(E v) : Enumeration((uint8)v) {};
    static void Load();
    static InputCouple &Current();
    static void Set(E v);
    bool IsDC() const { return (value == InputCouple::DC); }
};


struct Settings
{
    Channel::E   currentChannel;                // ������� �����
    int          levelSynch[Channel::Count][2]; // ������� �������������
    TypeSynch::E typeSynch[Channel::Count];     // ��� ������������� ��� ������� �� �������
    bool         showStatistics;                // ���������� �� ���������� �� FPS
};

extern Settings set;
