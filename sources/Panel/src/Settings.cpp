#include "defines.h"
#include "Settings.h"


Settings set =
{
    Channel::A,                         ///< currentChannel
    {
        {0, 0}, {0,0}    ///< levelSynch
    },
    { 
        TypeSynch::Manual, TypeSynch::Manual  ///< typeSynch
    }
};
