#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/PageModesA.h"
#include "Menu/Pages/PageModesB.h"
#include "Menu/Pages/PageModesC.h"
#include "Menu/Pages/PageModesD.h"
#include "Settings.h"


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
    static void FixPress();
    static Switch *switchTypeSynch;     ///< Указатель на переключатель типа синхронизации
};

