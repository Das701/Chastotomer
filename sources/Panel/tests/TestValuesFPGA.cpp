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
        String *result[TimeMeasure::Count];
    };

    // Channel::A

    //                                          1 ms     10 ms     100 ms      1 s      10 s      100 s
    String results0[TimeMeasure::Count]   = {  "0 Hz",  "0 Hz",    "0 Hz",     "0 Hz",  "0 Hz",   "0 Hz" };     // 0
    String results1[TimeMeasure::Count]   = {  "0 Hz",  "0,1 kHz", "0,01 kHz", "1 Hz",  "0,1 Hz", "0 Hz" };     // 1
    String results9[TimeMeasure::Count]   = {  "4 kHz", "0,5 kHz", "0,04 kHz", "4 Hz",  "0,4 Hz", "0,05 Hz" };  // 9

    String results10[TimeMeasure::Count]  = {  "5 kHz", "0,5 kHz", "0,05 kHz", "5 Hz",  "0,5 Hz", "0,05 Hz" };  // 10
    String results11[TimeMeasure::Count]  = {  "6 kHz", "0,6 kHz", "0,06 kHz", "5 Hz",  "0,6 Hz", "0,06 Hz" };  // 11
    String results99[TimeMeasure::Count]  = { "50 kHz", "5,0 kHz", "0,49 kHz", "50 Hz", "5,0 Hz", "0,49 Hz" };  // 99

    String results100[TimeMeasure::Count] = { "50 kHz", "5,0 kHz", "0,50 kHz", "50 Hz", "5,0 Hz", "0,50 Hz" };  // 100
    String results101[TimeMeasure::Count] = { "50 kHz" };                                                       // 101

    // Channel::B

    // Channel::C

    // Channel::D
}


static void Tests::ValuleFrequency_T_1()
{

}
