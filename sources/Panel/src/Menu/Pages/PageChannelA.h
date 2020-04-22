#pragma once
#include "Menu/MenuItems.h"


/// Открытый/закрытый вход
struct InputCouple : public Enumeration
{
    enum E
    {
        AC,         ///< Открытый
        DC          ///< Закрытый
    };

    explicit InputCouple(E v) : Enumeration((uint8)v) {};
};

/// Входное сопротивление
struct InputImpedance : public Enumeration
{
    enum E
    {
        _1MOmh,         ///< 1 МОм
        _50Omh          ///< 50 Ом
    };

    explicit InputImpedance(E v) : Enumeration((uint8)v) {};
};

/// ФНЧ
struct ModeFilter : public Enumeration
{
    enum E
    {
        On,             ///< Включен
        Off             ///< Выключен
    };

    explicit ModeFilter(E v) : Enumeration((uint8)v) {};
};

/// Фронт синхронизации
struct ModeFront : public Enumeration
{
    enum E
    {
        Front,          ///< Фронт
        Slice           ///< Срез
    };

    explicit ModeFront(E v) : Enumeration((uint8)v) {};
};

/// Делитель напряжения
struct Divider : public Enumeration
{
    enum E
    {
        _1,           ///< 1:1
        _10           ///< 1:10
    };

    explicit Divider(E v) : Enumeration((uint8)v) {};
};

/// Выбор типа синхронизации - ТТЛ или ЭСЛ
struct TypeSynch : public Enumeration
{
    enum E
    {
        Manual,         ///< Ручн
        TTL,            ///< ТТЛ
        ESL             ///< ЭСЛ
    };

    explicit TypeSynch(E v) : Enumeration((uint8)v) {};
};


class PageChannelA
{
public:
    static Page *self;
    static InputCouple couple;
    static InputImpedance impedance;
    static ModeFilter modeFilter;
    static ModeFront modeFront;
    static Divider divider;
    static TypeSynch typeSynch;

    static Switch *switchTypeSynch;     ///< Указатель на переключатель типа синхронизации
};

