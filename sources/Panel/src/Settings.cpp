#include "defines.h"
#include "Settings.h"


Settings set =
{
    Channel::A,                         ///< currentChannel
    {
        {0, 1200, -1240}, {0, 1200, -1240}    ///< levelSynch
    },
    { 
        TypeSynch::Manual, TypeSynch::Manual  ///< typeSynch
    }
};
