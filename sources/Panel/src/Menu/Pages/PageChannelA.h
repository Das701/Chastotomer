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
        Open,           ///< Открытый
        Closed          ///< Закрытый
    } value;

    explicit ModeOCI(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};

struct ModeImpedance
{
    enum E
    {
        MOhm1,          ///< 1 МОм
        Ohm50           ///< 50 Ом
    } value;

    explicit ModeImpedance(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};

struct ModeFilter
{
    enum E
    {
        On,             ///< Включен
        Off             ///< Выключен
    } value;

    explicit ModeFilter(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};

struct ModeFront
{
    enum E
    {
        Front,          ///< Фронт
        Slice           ///< Срез
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
        TTL,            ///< ТТЛ
        ESL             ///< ЭСЛ
    } value;

    explicit ModeSync(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};



class PageChannelA
{
public:
    static Page *self;
    /// Подсказки
    static HintA hintA;
    /// Режим измерения частоты
    static ModeOCI modeOCI;
    /// Режим измерения периода
    static ModeImpedance modeImpedance;
    /// Режим измерения длительности
    static ModeFilter modeFilter;
    /// Режим счёта импульсов
    static ModeFront modeFront;
    /// Период меток времени
    static ModeVD modeVD;
    /// Время счета
    static ModeSync modeSync;
};

