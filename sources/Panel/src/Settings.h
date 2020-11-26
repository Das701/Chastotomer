#pragma once
#include "Hardware/FreqMeter.h"
#include "Menu/MenuItems.h"


#define CURRENT_CHANNEL             (set.currentChannel)
#define CURRENT_CHANNEL_IS_A        (CURRENT_CHANNEL == Channel::A)
#define CURRENT_CHANNEL_IS_B        (CURRENT_CHANNEL == Channel::B)
#define CURRENT_CHANNEL_IS_A_OR_B   (CURRENT_CHANNEL_IS_A || CURRENT_CHANNEL_IS_B)
#define CURRENT_CHANNEL_IS_C        (CURRENT_CHANNEL == Channel::C)
#define CURRENT_CHANNEL_IS_D        (CURRENT_CHANNEL == Channel::D)

#define TYPE_SYNCH(ch)              (set.typeSynch[(ch)])
#define TYPE_SYNCH_A                (TYPE_SYNCH(Channel::A))
#define TYPE_SYNCH_B                (TYPE_SYNCH(Channel::B))

#define LEVEL_SYNCH(ch)             (set.levelSynch[ch][TYPE_SYNCH(ch)])
#define LEVEL_SYNCH_A               (LEVEL_SYNCH(Channel::A))
#define LEVEL_SYNCH_B               (LEVEL_SYNCH(Channel::B))


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

    // Загрузить текущий канал в аппаратуру
    static void LoadCurrentToFPGA();
};


struct LevelSynch
{
    // Изменить уровень синхронизации на заданное количество милливольт (делитель не учитывается. Он учитывается только при выводе на дисплей)
    static void Change(int delta);
};


// Открытый/закрытый вход
struct InputCouple : public Enumeration
{
    enum E
    {
        AC,         // Открытый
        DC,         // Закрытый
        Count
    };

    explicit InputCouple(E v) : Enumeration((uint8)v) {};
    static void LoadToFPGA();
    static const InputCouple &Current();
    static void Set(E v);
    bool IsDC() const { return (value == InputCouple::DC); }

private:
    static const InputCouple null;
};


// Фронт синхронизации
struct ModeFront : public Enumeration
{
    enum E
    {
        Front,          // Фронт
        Slice,          // Срез
        Count
    };

    explicit ModeFront(E v) : Enumeration((uint8)v) {};
    bool IsFront() const { return (value == Front); }

    static const ModeFront &Current();
    static void LoadToFPGA();

private:
    static const ModeFront null;
};


// Выбор типа синхронизации - ТТЛ или ЭСЛ
struct TypeSynch : public Enumeration
{
    enum E
    {
        Manual,         // Ручн
        Holdoff,        // Holdoff
        Count
    };

    explicit TypeSynch(E v) : Enumeration((uint8)v) {};
    bool IsHoldoff() const { return (value == Holdoff); }
    bool IsManual() const { return (value == Manual); }

    // Загрузить в аппаратуру
    static void LoadToFPGA();

    // Тип синхронизации на текущем канале
    static const TypeSynch &Current();

private:
    static const TypeSynch null;
};


struct Settings
{
    Channel::E   currentChannel;                // Текущий канал
    int          levelSynch[Channel::Count][2]; // Уровень синхронизации
    TypeSynch::E typeSynch[Channel::Count];     // Тип синхронизации для каждого из каналов
    bool         showStatistics;                // Показывать ли статистику по FPS
};

extern Settings set;
