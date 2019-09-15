#pragma once
#include "Menu/MenuItems.h"


/// ��������/�������� ����
struct ChannelInput : public Enumeration
{
    enum E
    {
        Open,           ///< ��������
        Closed          ///< ��������
    };

    explicit ChannelInput(E v) : Enumeration((uint8)v) {};
};

/// ������� �������������
struct InputImpedance : public Enumeration
{
    enum E
    {
        _1MOmh,         ///< 1 ���
        _50Omh          ///< 50 ��
    };

    explicit InputImpedance(E v) : Enumeration((uint8)v) {};
};

/// ���
struct ModeFilter : public Enumeration
{
    enum E
    {
        On,             ///< �������
        Off             ///< ��������
    };

    explicit ModeFilter(E v) : Enumeration((uint8)v) {};
};

/// ����� �������������
struct ModeFront : public Enumeration
{
    enum E
    {
        Front,          ///< �����
        Slice           ///< ����
    };

    explicit ModeFront(E v) : Enumeration((uint8)v) {};
};

/// �������� ����������
struct Divider : public Enumeration
{
    enum E
    {
        _1,           ///< 1:1
        _10           ///< 1:10
    };

    explicit Divider(E v) : Enumeration((uint8)v) {};
};

/// ����� ���� ������������� - ��� ��� ���
struct TypeSynch : public Enumeration
{
    enum E
    {
        TTL,            ///< ���
        ESL             ///< ���
    };

    explicit TypeSynch(E v) : Enumeration((uint8)v) {};
};


class PageChannelA
{
public:
    static Page *self;
    static ChannelInput channelInput;
    static InputImpedance inputImpedance;
    static ModeFilter modeFilter;
    static ModeFront modeFront;
    static Divider divider;
    static TypeSynch typeSynch;

    static Switch *switchTypeSynch;     ///< ��������� �� ������������� ���� �������������
};

