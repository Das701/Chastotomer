#pragma once
#include "Menu/Pages/PageChannelA.h"


#define CURRENT_CHANNEL         (set.currentChannel)
#define CURRENT_CHANNEL_IS_A    (CURRENT_CHANNEL == Channel::A)
#define CURRENT_CHANNEL_IS_B    (CURRENT_CHANNEL == Channel::B)

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

struct Settings
{
    int          levelSynch[Channel::Count][2]; ///< Уровень синхронизации
    Channel::E   currentChannel;                ///< Текущий канал
    TypeSynch::E typeSynch[Channel::Count];     ///< Тип синхронизации для каждого из каналов
};

extern Settings set;
