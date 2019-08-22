#pragma once
#include "defines.h"


struct Control
{
    enum E
    {
        GovButton,
        Mode,
        Indication,
        _3,
        Left,
        Right,
        _6,
        _7,
        _8,
        _9,
        _A,
        _B,
        _C,
        _D,
        _F5,
        _ESC,
        _Left,
        _Right,
        GovLeft,
        GovRight,
        _Channel,
        Test,
        Auto,
        _NULL
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
    } action;

    Control(E v = _NULL, Action::E a = Action::Press) : value(v), action(a) {};

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
