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
        {0, "0 Hz"},
        {1, "0 Hz"},
        {(uint)-1}
    };

    TestStruct arrayA_10ms[] =
    {
        {0, "0 Hz"},
        {0, "0 Hz"},
        {(uint)-1}
    };

    TestStruct arrayA_100ms[] =
    {
        {0, "0 Hz"},
        {0, "0 Hz"},
        {(uint)-1}
    };

    TestStruct arrayA_1s[] =
    {
        {0, "0 Hz"},
        {0, "0 Hz"},
        {(uint)-1}
    };

    TestStruct arrayA_10s[] =
    {
        {0, "0 Hz"},
        {0, "0 Hz"},
        {(uint)-1}
    };

    TestStruct arrayA_100s[] =
    {
        {0, "0 Hz"},
        {0, "0 Hz"},
        {(uint)-1}
    };

    // Channel::B

    // Channel::C

    // Channel::D
}


static void Tests::ValuleFrequency_T_1()
{

}
