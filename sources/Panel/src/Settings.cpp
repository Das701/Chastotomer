#include "defines.h"
#include "Settings.h"


Settings set =
{
    Channel::A,                         ///< currentChannel
    {
        {0, 1}, {0, 1}    ///< levelSynch
    },
    { 
        TypeSynch::Manual, TypeSynch::Manual  ///< typeSynch
    }
};
