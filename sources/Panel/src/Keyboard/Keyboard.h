#pragma once
#include "defines.h"
#include "Utils/String.h"


struct Control
{
    enum E
    {
/*  0 */   GovButton,
/*  1 */   Mode,
/*  2 */   Indication,
/*  3 */   Left,
/*  4 */   Right,
/*  5 */   Channels,
/*  6 */   Enter,
/*  7 */   Service,
/*  8 */   GovLeft,
/*  9 */   GovRight,
/* 10 */   Test,
/* 11 */   Auto,
/* 12 */   None,
/* 13 */   Count
    } value;

    struct Action
    {
        enum E
        {
            Press,
            Release,
            Long
        } value;
        Action(E v) : value(v) {};
        bool IsPress() const { return value == Press; };
    } action;

    Control(E v = None, Action::E a = Action::Press) : value(v), action(a) {};

    String Name() const;

    bool IsRotateGovernor() const { return (value == GovLeft) || (value == GovRight); }
};

namespace Keyboard
{
    void Init();
    // Возвращает true, если есть событыия
    bool Empty();
    // Возвращает следующее событие
    Control NextControl();
};
