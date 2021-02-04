#pragma once


#define SYMBOL_AC                  0x80
#define SYMBOL_AC_IN_QUOTES       "\x80"
#define SYMBOL_DC                  0x81
#define SYMBOL_DC_IN_QUOTES       "\x81"
#define SYMBOL_FRONT               0x82     // Фронт
#define SYMBOL_FRONT_IN_QUOTES    "\x82"
#define SYMBOL_RISE                0x83     // Срез
#define SYMBOL_RISE_IN_QUOTES     "\x83"


struct UGO
{
    static char *OVERLAPPED;    // Индикация переполнения
    static char *EMPTY_VALUE;   // Выводится, когда значение ещё не получено после засылки значения
    static char *DivNULL;       // Индикация деления на ноль
};
