#pragma once
#include "defines.h"


struct Control
{
    enum E
    {
/*  0 */   GovButton,
/*  1 */   Mode,
/*  2 */   Indication,
/*  3 */   Left,
/*  4 */   Right,
/*  5 */   A,
/*  6 */   B,
/*  7 */   C,
/*  8 */   D,
/*  9 */   Esc,
/* 10 */   GovLeft,
/* 11 */   GovRight,
/* 12 */   Test,
/* 13 */   Auto,
/* 14 */   None,
/* 15 */   Count
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

    const char *Name();
};

namespace Keyboard
{
    void Init();
    /// Возвращает true, если есть событыия
    bool Empty();
    /// Возвращает следующее событие
    Control NextControl();
};
