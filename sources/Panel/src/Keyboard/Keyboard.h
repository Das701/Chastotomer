#pragma once
#include "defines.h"

namespace Keyboard
{
    void Init();
};

struct Control
{
    enum E
    {
        _0,
        _1,
        _2,
        _3,
        _4,
        _5,
        _6,
        _7,
        _8,
        _9,
        _F1,
        _F2,
        _F3,
        _F4,
        _F5,
        _ESC,
        _Left,
        _Right,
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
    operator uint8() const { return (uint8)value; };
};
