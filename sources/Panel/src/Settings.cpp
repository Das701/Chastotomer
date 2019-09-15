#include "defines.h"
#include "Settings.h"


Settings set =
{
    {
        {1200, -1240}, {1200, -1240}    ///< levelSynch
    },
    Channel::A,                         ///< currentChannel
    { 
        TypeSynch::TTL, TypeSynch::TTL  ///< typeSynch
    }
};
