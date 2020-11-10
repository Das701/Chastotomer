#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PageModesA.h"
#include "Menu/Pages/Modes/PageModesB.h"
#include "Menu/Pages/Modes/PageModesC.h"
#include "Menu/Pages/Modes/PageModesD.h"
#include "Settings.h"


// ¬ходное сопротивление
struct InputImpedance : public Enumeration
{
    enum E
    {
        _1MOmh,         // 1 ћќм
        _50Omh          // 50 ќм
    };

    explicit InputImpedance(E v) : Enumeration((uint8)v) {};

    static InputImpedance &Current();
    bool Is_1MOhm() const { return (value == _1MOmh); }
    bool Is_50Ohm() const { return (value == _50Omh); }
};


// ƒелитель напр€жени€
struct Divider : public Enumeration
{
    enum E
    {
        _1,           // 1:1
        _10           // 1:10
    };

    explicit Divider(E v) : Enumeration((uint8)v) {};
    static Divider &Current();
    bool Is1() const { return (value == _1); }
    int ToAbs() const { return Is1() ? 1 : 10; }
};


struct PageSettingsA
{
    static Page *self;
    static InputCouple couple;
    static InputImpedance impedance;
    static ModeFilter modeFilter;
    static ModeFront modeFront;
    static Divider divider;
    static TypeSynch typeSynch;
    static void FixPress();
    static Switch *switchTypeSynch;     // ”казатель на переключатель типа синхронизации
};


struct PageSettingsB
{
    static Page *self;
    static InputCouple couple;
    static InputImpedance impedance;
    static ModeFilter modeFilter;
    static ModeFront modeFront;
    static Divider divider;
    static TypeSynch typeSynch;
    static void FixPress();
    static Switch *switchTypeSynch;     // ”казатель на переключатель типа синхронизации
};


struct PageSettingsC
{
    static Page *self;
};

struct PageSettingsD
{
    static Page *self;
};
