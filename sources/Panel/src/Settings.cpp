#include "defines.h"
#include "Settings.h"


Settings set =
{
    Channel::A,                         ///< currentChannel
    {
        {1200, -1240}, {1200, -1240}    ///< levelSynch
    },
    { 
        TypeSynch::TTL, TypeSynch::TTL  ///< typeSynch
    }
};
