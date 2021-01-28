#include "defines.h"
#include "Tests.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Menu/Pages/Modes/Modes.h"


namespace Tests
{
    static void ValueFrequency_Frequency();

    static void ValuleFrequency_T_1();
}


void Tests::ValuesFPGA()
{
    ValueFrequency_Frequency();

    ValuleFrequency_T_1();
}


static void Tests::ValueFrequency_Frequency()
{
    struct TestStruct
    {
        uint   counter;
        String result;
    };

    // Channel::A

    TestStruct arrayA_1ms[] =
    {
        {0,  "0 Hz"},
        {1,  "0 Hz"},
        {9,  "4 kHz"},
        {10, "5 kHz"},      // ?
        {11, "6 kHz"},      // ?
        {99, "50 kHz"},
        {100, "50 kHz"},    // ?
        {101, "50 kHz"},    // ?
        {(uint)-1}
    };

    TestStruct arrayA_10ms[] =
    {
        {0,  "0 Hz"},
        {1,  "0,1 kHz"},
        {9,  "0,5 kHz"},
        {10, "0,5 kHz"},    // ?
        {11, "0,6 kHz"},    // ?
        {99, "5,0 kHz"},
        {100, "5,0 kHz"},   // ?
        {(uint)-1}
    };

    TestStruct arrayA_100ms[] =
    {
        {0,  "0 Hz"},
        {1,  "0,01 kHz"},
        {9,  "0,04 kHz"},
        {10, "0,05 kHz"},   // ?
        {11, "0,06 kHz"},   // ?
        {99, "0,49 kHz"},
        {100, "0,50 kHz"},  // ?
        {(uint)-1}
    };

    TestStruct arrayA_1s[] =
    {
        {0,  "0 Hz"},
        {1,  "1 Hz"},
        {9,  "4 Hz"},
        {10, "5 Hz"},   // ?
        {11, "5 Hz"},   // ?
        {99, "50 Hz"},
        {100, "50 Hz"}, // ?
        {(uint)-1}
    };

    TestStruct arrayA_10s[] =
    {
        {0,  "0 Hz"},
        {1,  "0,1 Hz"},
        {9,  "0,4 Hz"},
        {10, "0,5 Hz"},     // ?
        {11, "0,6 Hz"},     // ?
        {99, "5,0 Hz"},     // ?
        {100, "5,0 Hz"},    // ?
        {(uint)-1}
    };

    TestStruct arrayA_100s[] =
    {
        {0,  "0 Hz"},
        {1,  "0 Hz"},
        {9,  "0,05 Hz"},
        {10, "0,05 Hz"},    // ?
        {11, "0,06 Hz"},    // ?
        {99, "0,49 Hz"},    // ?
        {100, "0,50 Hz"},   // ?
        {(uint)-1}
    };

    // Channel::B

    // Channel::C

    // Channel::D
}


static void Tests::ValuleFrequency_T_1()
{

}
