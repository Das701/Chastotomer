#include "defines.h"
#include "Tests.h"
#include "Calculate/MathFPGA.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Menu/Pages/Modes/Modes.h"
#include <cstring>


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
    // Channel::A

    //                                                1 ms              10 ms             100 ms            1 s               10 s              100 s             1000s
    String results_0[TimeMeasure::Count]          = { "0 Hz",           "0 Hz",           "0 Hz",           "0 Hz",           "0 Hz",           "0 Hz",           "0 Hz"};         // 0
    //                                                                                                      0.001kHz          0.0001Hz          0.00001
    String results_2[TimeMeasure::Count]          = { "1 kHz",          "0,1 kHz",        "0,01 kHz",       "1 Hz",           "0,1 Hz",         "0,01 Hz",        "1 mHz" };      // 1
    //                                                                                                      0.009kHz          0.0009kHz         0.00009
    String results_18[TimeMeasure::Count]         = { "9 kHz",          "0,9 kHz",        "0,09 kHz",       "9 Hz",           "0,9 Hz",         "0,09 Hz",        "9 mHz"};       // 9
                                                                                                                                                
    //                                                                                                      0.010kHz          0.0010kHz         0.00010kHz
    String results_20[TimeMeasure::Count]         = { "10 kHz",         "1.0 kHz",        "0,10 kHz",       "10 Hz",          "10 Hz",          "0,10 Hz",        "10 mHz"};       // 10
    //                                                                                                      0.011kHz          0.0011kHz         0.00011
    String results_22[TimeMeasure::Count]         = { "11 kHz",         "1,1 kHz",        "0,11 kHz",       "11 Hz",          "1,1 Hz",         "0,11 Hz",        "11 mHz"};       // 11
    //                                                                                                      0.099kHz          0.0099kHz         0.00099kHz
    String results_198[TimeMeasure::Count]        = { "99 kHz",         "9,9 kHz",        "0,99 kHz",       "99 Hz",          "9,9 Hz",         "0,99 Hz",        "90 mHz" };      // 99
                                                                                                                                                
    //                                                                                                      0.100kHz          0.0100kHz         0.00100kHz
    String results_200[TimeMeasure::Count]        = { "100 kHz",        "10,0 kHz",       "1,00 kHz",       "100 Hz",         "10,0 Hz",        "1,00 Hz",        "100 mHz" };      // 100
    String results_202[TimeMeasure::Count]        = { "101 kHz",        "10,1 kHz",       "1,01 kHz",       "101 Hz",         "10,1 Hz",        "1,01 Hz",        "101 mHz" };      // 101
    String results_1998[TimeMeasure::Count]       = { "999 kHz",        "99,9 kHz",       "9,99 kHz",       "999 Hz",         "99,9 Hz",        "9,99 Hz",        "999 mHz"};       // 999
                                                                                                                                                
    String results_2000[TimeMeasure::Count]       = { "1,000 MHz",      "100,0 kHz",      "10,00 kHz",      "1,000 kHz",      "100,0 Hz",       "10,00 Hz",       "1,000 Hz" };     // 1 000
    String results_2002[TimeMeasure::Count]       = { "1,001 MHz",      "100,1 kHz",      "10,01 kHz",      "1,001 kHz",      "100,1 Hz",       "10,01 Hz",       "1,001 Hz" };     // 1 001
    String results_19998[TimeMeasure::Count]      = { "9,999 MHz",      "999,9 kHz",      "99,99 kHz",      "9,999 kHz",      "999,9 Hz",       "99,99 Hz",       "9,999 Hz" };     // 9 999
                                                                                                                                                
    String results_20000[TimeMeasure::Count]      = { "10,000 MHz",     "1,0000 MHz",     "100,00 kHz",     "10,000 kHz",     "1,0000 kHz",     "100,00 Hz",      "10,000 Hz" };     // 10 000
    String results_20002[TimeMeasure::Count]      = { "10,001 MHz",     "1,0001 MHz",     "100,01 kHz",     "10,001 kHz",     "1,0001 kHz",     "100,01 Hz",      "10,001 Hz" };     // 10 001
    String results_199998[TimeMeasure::Count]     = { "99,999 MHz",     "9,9999 MHz",     "999,99 kHz",     "99,999 kHz",     "9,9999 kHz",     "999,99 Hz",      "99,999 Hz" };     // 99 999
                                                                                                                                                
    String results_200002[TimeMeasure::Count]     = { "100,001 MHz",    "10,0001 MHz",    "1,00001 MHz",    "100,001 kHz",    "10,0001 kHz",    "1,00001 kHz",    "100,001 kHz" };    // 100 001
    String results_1999998[TimeMeasure::Count]    = { "999,999 MHz",    "99,9999 MHz",    "9,99999 MHz",    "999,999 kHz",    "99,9999 kHz",    "9,99999 kHz",    "999,999 Hz" };    // 999 999
                                                                                                                                                
    String results_2000002[TimeMeasure::Count]    = { "1,000001 GHz",   "100,0001 MHz",   "10,00001 MHz",   "1,000001 MHz",   "100,0001 kHz",   "10,00001 kHz",   "1,000001 kHz" };   // 1 000 001
    String results_19999998[TimeMeasure::Count]   = { "9,999999 GHz",   "999,9999 MHz",   "99,99999 MHz",   "9,999999 MHz",   "999,9999 kHz",   "99,99999 kHz",   "9,999999 kHz" };   // 9 999 999
                                                                                                                                                
    String results_20000002[TimeMeasure::Count]   = { "10,000001 GHz",  "1,0000001 GHz",  "100,00001 MHz",  "10,000001 MHz",  "1,0000001 MHz",  "100,00001 kHz",  "10,000001 kHz" };  // 10 000 001
    String results_199999998[TimeMeasure::Count]  = { "99,999999 GHz",  "9,9999999 GHz",  "999,99999 MHz",  "99,999999 MHz",  "9,9999999 MHz",  "999,99999 kHz",  "99,999999 kHz" };  // 99 999 999
                                                  
    String results_200000002[TimeMeasure::Count]  = { "100,000001 GHz", "10,0000001 GHz", "1,00000001 GHz", "100,000001 MHz", "10,0000001 MHz", "1,00000001 MHz", "100,000001 kHz" }; // 100 000 001
    String results_1999999998[TimeMeasure::Count] = { "999,999999 GHz", "99,9999999 GHz", "9,99999999 GHz", "999,999999 MHz", "99,9999999 MHz", "9,99999999 MHz", "999,999999 kHz" }; // 999 999 999

    uint counters[] = {
        0, 2, 18,
        20, 22, 198,
        200, 202, 1998,
        2000, 2002, 19998,
        20000, 20002, 199998,
        200002, 1999998,
        2000002, 19999998,
        20000002, 199999998,
        200000002, 1999999998,
        (uint)-1
    };

    String *results[] =
    {
        results_0, results_2, results_18,
        results_20, results_22, results_198,
        results_200, results_202, results_1998,
        results_2000, results_2002, results_19998,
        results_20000, results_20002, results_199998,
        results_200002, results_1999998,
        results_2000002, results_19999998,
        results_20000002, results_199999998,
        results_200000002, results_1999999998
    };

    Channel *current = Channel::Current();
    uint8 type = Channel::Current()->mod.typeMeasure.value;
    uint8 mode = Channel::Current()->mod.modeFrequency.value;
    uint8 timeMeasure = Channel::Current()->mod.timeMeasure.value;

    Channel::SetCurrent(Channel::A);
    Channel::A->mod.typeMeasure.value = TypeMeasure::Frequency;
    Channel::A->mod.modeFrequency.value = ModeFrequency::Frequency;

    for (int i = 0; counters[i] != (uint)-1; i++)
    {
        uint counter = counters[i];

        String *result = results[i];

        int numString = 0;

        for (uint8 time = 0; time < TimeMeasure::Count; time++)
        {
            Channel::A->mod.timeMeasure.value = time;

            MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::MainCounters, counter, 0);

            char *value_str = MathFPGA::Measure::valueFPGA->value.c_str();
            char *standard_str = (*result).c_str();
            numString = numString;

            if (std::strcmp(value_str, standard_str) != 0)
            {
                FailExit();
            }

            result++;
            numString++;
        }
    }

    Channel::SetCurrent(current);
    Channel::Current()->mod.typeMeasure.value = type;
    Channel::Current()->mod.modeFrequency.value = mode;
    Channel::Current()->mod.timeMeasure.value = timeMeasure;

    // Channel::B

    // Channel::C

    // Channel::D
}


static void Tests::ValuleFrequency_T_1()
{

}
