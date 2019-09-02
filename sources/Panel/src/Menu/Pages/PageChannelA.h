#pragma once
#include "Menu/MenuItems.h"



struct HintA
{
    enum E
    {
        OCI,      
        Impedance,           
        Filter,        
        Front,     
        VD,
        Sync 
    } value;

    explicit HintA(E v) : value(v) {};
    operator int() { return (int)value; };
};

struct ModeOCI
{
    enum E
    {
        Open,           ///< ��������
        Closed          ///< ��������
    } value;

    explicit ModeOCI(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};

struct ModeImpedance
{
    enum E
    {
        MOhm1,          ///< 1 ���
        Ohm50           ///< 50 ��
    } value;

    explicit ModeImpedance(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};

struct ModeFilter
{
    enum E
    {
        On,             ///< �������
        Off             ///< ��������
    } value;

    explicit ModeFilter(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};

struct ModeFront
{
    enum E
    {
        Front,          ///< �����
        Slice           ///< ����
    } value;

    explicit ModeFront(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};

struct ModeVD
{
    enum E
    {
        A1_1,           ///< 1:1
        A1_10           ///< 1:10
    } value;

    explicit ModeVD(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};

struct ModeSync
{
    enum E
    {
        TTL,            ///< ���
        ESL             ///< ���
    } value;

    explicit ModeSync(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};



class PageChannelA
{
public:
    static Page *self;
    /// ���������
    static HintA hintA;
    /// ����� ��������� �������
    static ModeOCI modeOCI;
    /// ����� ��������� �������
    static ModeImpedance modeImpedance;
    /// ����� ��������� ������������
    static ModeFilter modeFilter;
    /// ����� ����� ���������
    static ModeFront modeFront;
    /// ������ ����� �������
    static ModeVD modeVD;
    /// ����� �����
    static ModeSync modeSync;
};

