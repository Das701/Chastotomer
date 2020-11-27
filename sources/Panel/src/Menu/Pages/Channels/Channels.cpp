#include "defines.h"
#include "Menu/Pages/Channels/Channels.h"


Channel::E Channel::current = Channel::A;


Page *Channel::PageForChannel(Channel::E channel)
{
    Page *pages[Channel::Count] =
    {
        PageModesA::self,
        PageModesB::self,
        PageModesC::self,
        PageModesD::self
    };

    return pages[channel];
}
