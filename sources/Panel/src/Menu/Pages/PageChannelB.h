#pragma once
#include "Menu/MenuItems.h"

#include "Menu/MenuItems.h"


/// Открытый/закрытый вход
struct ChannelInputB : public Enumeration
{
    enum E
    {
        Open,           ///< Открытый
        Closed          ///< Закрытый
    };

    explicit ChannelInputB(E v) : Enumeration((uint8)v) {};
};

/// Входное сопротивление
struct InputImpedanceB : public Enumeration
{
    enum E
    {
        _1MOmh,         ///< 1 МОм
        _50Omh          ///< 50 Ом
    };

    explicit InputImpedanceB(E v) : Enumeration((uint8)v) {};
};

/// ФНЧ
struct ModeFilterB : public Enumeration
{
    enum E
    {
        On,             ///< Включен
        Off             ///< Выключен
    };

    explicit ModeFilterB(E v) : Enumeration((uint8)v) {};
};

/// Фронт синхронизации
struct ModeFrontB : public Enumeration
{
    enum E
    {
        Front,          ///< Фронт
        Slice           ///< Срез
    };

    explicit ModeFrontB(E v) : Enumeration((uint8)v) {};
};

/// Делитель напряжения
struct DividerB : public Enumeration
{
    enum E
    {
        _1,           ///< 1:1
        _10           ///< 1:10
    };

    explicit DividerB(E v) : Enumeration((uint8)v) {};
};

/// Выбор уровня синхронизации - ТТЛ или ЭСЛ
struct LevelSynchB : public Enumeration
{
    enum E
    {
        TTL,            ///< ТТЛ
        ESL             ///< ЭСЛ
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

