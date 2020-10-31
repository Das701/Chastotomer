#pragma once
#include "Menu/MenuItems.h"


#define CURRENT_CHANNEL         (set.currentChannel)
#define CURRENT_CHANNEL_IS_A    (CURRENT_CHANNEL == Channel::A)
#define CURRENT_CHANNEL_IS_B    (CURRENT_CHANNEL == Channel::B)
#define CURRENT_CHANNEL_IS_C    (CURRENT_CHANNEL == Channel::C)
#define CURRENT_CHANNEL_IS_D    (CURRENT_CHANNEL == Channel::D)

#define TYPE_SYNCH(ch)          (set.typeSynch[(ch)])
#define TYPE_SYNCH_A            (TYPE_SYNCH(Channel::A))
#define TYPE_SYNCH_B            (TYPE_SYNCH(Channel::B))

#define LEVEL_SYNCH(ch)         (set.levelSynch[ch][TYPE_SYNCH(ch)])
#define LEVEL_SYNCH_A           (LEVEL_SYNCH(Channel::A))
#define LEVEL_SYNCH_B           (LEVEL_SYNCH(Channel::B))


struct Channel
{
    enum E
    {
        A,
        B,
        C,
        D,
        Count
    };
};

// Открытый/закрытый вход
struct InputCouple : public Enumeration
{
    enum E
    {
        AC,         // Открытый
        DC          // Закрытый
    };

    explicit InputCouple(E v) : Enumeration((uint8)v) {};
    static void Load();
    static InputCouple &Current();
    static void Set(E v);
    bool IsDC() const { return (value == InputCouple::DC); }
};


// ФНЧ
struct ModeFilter : public Enumeration
{
    enum E
    {
        On,             // Включен
        Off             // Выключен
    };

    explicit ModeFilter(E v) : Enumeration((uint8)v) {};
    static void Load();
    static ModeFilter& Current();
    static void Set(E v);
    bool IsOff() const { return (value == ModeFilter::Off); }
};


// Выбор типа синхронизации - ТТЛ или ЭСЛ
struct TypeSynch : public Enumeration
{
    enum E
    {
        Manual,         // Ручн
        Holdoff         // Holdoff
    };

    explicit TypeSynch(E v) : Enumeration((uint8)v) {};
};


struct Settings
{
    Channel::E   currentChannel;                // Текущий канал
    int          levelSynch[Channel::Count][2]; // Уровень синхронизации
    TypeSynch::E typeSynch[Channel::Count];     // Тип синхронизации для каждого из каналов
};

extern Settings set;
