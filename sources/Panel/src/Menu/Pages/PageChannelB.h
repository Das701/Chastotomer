#pragma once
#include "Menu/MenuItems.h"

#include "Menu/MenuItems.h"


/// ��������/�������� ����
struct ChannelInputB : public Enumeration
{
    enum E
    {
        Open,           ///< ��������
        Closed          ///< ��������
    };

    explicit ChannelInputB(E v) : Enumeration((uint8)v) {};
};

/// ������� �������������
struct InputImpedanceB : public Enumeration
{
    enum E
    {
        _1MOmh,         ///< 1 ���
        _50Omh          ///< 50 ��
    };

    explicit InputImpedanceB(E v) : Enumeration((uint8)v) {};
};

/// ���
struct ModeFilterB : public Enumeration
{
    enum E
    {
        On,             ///< �������
        Off             ///< ��������
    };

    explicit ModeFilterB(E v) : Enumeration((uint8)v) {};
};

/// ����� �������������
struct ModeFrontB : public Enumeration
{
    enum E
    {
        Front,          ///< �����
        Slice           ///< ����
    };

    explicit ModeFrontB(E v) : Enumeration((uint8)v) {};
};

/// �������� ����������
struct DividerB : public Enumeration
{
    enum E
    {
        _1,           ///< 1:1
        _10           ///< 1:10
    };

    explicit DividerB(E v) : Enumeration((uint8)v) {};
};

/// ����� ������ ������������� - ��� ��� ���
struct LevelSynchB : public Enumeration
{
    enum E
    {
        TTL,            ///< ���
        ESL             ///< ���
    };

    explicit LevelSynchB(E v) : Enumeration((uint8)v) {};
};


class PageChannelB
{
public:
    static Page *self;
    static ChannelInputB channelInput;
    static InputImpedanceB inputImpedance;
    static ModeFilterB modeFilter;
    static ModeFrontB modeFront;
    static DividerB divider;
    static LevelSynchB levelSynch;
};

