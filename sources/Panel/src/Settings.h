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
    int        syncValue;       ///< ������� �������������
    Channel::E currentChannel;  ///< ������� �����
};

extern Settings set;
