#pragma once

#define CURRENT_CHANNEL_IS_A

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
    int        syncValue;       ///< Уровень синхронизации
    Channel::E currentChannel;  ///< Текущий канал
};

extern Settings set;
