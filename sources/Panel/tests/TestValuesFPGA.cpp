#include "defines.h"
#include "Tests.h"
#include "Calculate/ValuesFPGA.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Utils/StringUtils.h"
#include <cstring>


namespace Tests
{
    namespace Frequency
    {
        static void Test();

        namespace Frequency
        {
            static void Test();

            static void TestChannelsCD();
        }

        namespace T_1
        {
            static void Test();
        }
    }

    namespace Period
    {
        static void Test();

        namespace Period
        {
            static void Test();
        }

        namespace F_1
        {
            static void Test();
        }
    }

    static void StoreSettings(Channel *channel);

    static void RestoreSettings(Channel *channel);

    static Channel *current = nullptr;
    static uint8 type = 0;
    static uint8 modeFrequency = 0;
    static uint8 modePeriod = 0;
    static uint8 timeMeasure = 0;
    static uint8 numberPeriods = 0;
    static uint8 timeLabels = 0;
}


void Tests::ValuesFPGA()
{
    Frequency::Test();

    Period::Test();
}


static void Tests::Frequency::Test()
{
    Frequency::Test();
    
    T_1::Test();
}


static void Tests::Period::Test()
{
    Period::Test();

    F_1::Test();
}


static char *GetMathValue()
{
    char *result = ValueFPGA::GiveStringValue();

#ifndef WIN32
    SU::ReplaceSymbol(result, '.', ',');
#endif

    return result;
}


static void Tests::Frequency::Frequency::Test()
{
    typedef char *pChar;

    typedef char *Array6[TimeMeasure::Count];

    // Channel::A

    //                                                1 ms              10 ms             100 ms            1 s               10 s              100 s             1000s
    Array6 results_0          = { "0 Hz",           "0 Hz",           "0 Hz",           "0 Hz",           "0 Hz",           "0 Hz",           "0 Hz"};         // 0
    Array6 results_2          = { "1 kHz",          "0,1 kHz",        "0,01 kHz",       "1 Hz",           "0,1 Hz",         "0,01 Hz",        "1 mHz" };      // 1
    Array6 results_18         = { "9 kHz",          "0,9 kHz",        "0,09 kHz",       "9 Hz",           "0,9 Hz",         "0,09 Hz",        "9 mHz"};       // 9
                                                                                                                                                
    Array6 results_20         = { "10 kHz",         "1,0 kHz",        "0,10 kHz",       "10 Hz",          "1,0 Hz",         "0,10 Hz",        "10 mHz" };       // 10
    Array6 results_22         = { "11 kHz",         "1,1 kHz",        "0,11 kHz",       "11 Hz",          "1,1 Hz",         "0,11 Hz",        "11 mHz" };       // 11
    Array6 results_198        = { "99 kHz",         "9,9 kHz",        "0,99 kHz",       "99 Hz",          "9,9 Hz",         "0,99 Hz",        "99 mHz" };      // 99
                                                                                                                                                
    Array6 results_200        = { "100 kHz",        "10,0 kHz",       "1,00 kHz",       "100 Hz",         "10,0 Hz",        "1,00 Hz",        "100 mHz" };      // 100
    Array6 results_202        = { "101 kHz",        "10,1 kHz",       "1,01 kHz",       "101 Hz",         "10,1 Hz",        "1,01 Hz",        "101 mHz" };      // 101
    Array6 results_1998       = { "999 kHz",        "99,9 kHz",       "9,99 kHz",       "999 Hz",         "99,9 Hz",        "9,99 Hz",        "999 mHz" };       // 999
                                                                                                                                                
    Array6 results_2000       = { "1,000 MHz",      "100,0 kHz",      "10,00 kHz",      "1,000 kHz",      "100,0 Hz",       "10,00 Hz",       "1,000 Hz" };     // 1 000
    Array6 results_2002       = { "1,001 MHz",      "100,1 kHz",      "10,01 kHz",      "1,001 kHz",      "100,1 Hz",       "10,01 Hz",       "1,001 Hz" };     // 1 001
    Array6 results_19998      = { "9,999 MHz",      "999,9 kHz",      "99,99 kHz",      "9,999 kHz",      "999,9 Hz",       "99,99 Hz",       "9,999 Hz" };     // 9 999
                                                                                                                                                
    Array6 results_20000      = { "10,000 MHz",     "1,0000 MHz",     "100,00 kHz",     "10,000 kHz",     "1,0000 kHz",     "100,00 Hz",      "10,000 Hz" };     // 10 000
    Array6 results_20002      = { "10,001 MHz",     "1,0001 MHz",     "100,01 kHz",     "10,001 kHz",     "1,0001 kHz",     "100,01 Hz",      "10,001 Hz" };     // 10 001
    Array6 results_199998     = { "99,999 MHz",     "9,9999 MHz",     "999,99 kHz",     "99,999 kHz",     "9,9999 kHz",     "999,99 Hz",      "99,999 Hz" };     // 99 999
                                                                                                                                                
    Array6 results_200002     = { "100,001 MHz",    "10,0001 MHz",    "1,00001 MHz",    "100,001 kHz",    "10,0001 kHz",    "1,00001 kHz",    "100,001 Hz" };    // 100 001
    Array6 results_1999998    = { "999,999 MHz",    "99,9999 MHz",    "9,99999 MHz",    "999,999 kHz",    "99,9999 kHz",    "9,99999 kHz",    "999,999 Hz" };    // 999 999

    Array6 results_2000002    = { "1,000001 GHz",   "100,0001 MHz",   "10,00001 MHz",   "1,000001 MHz",   "100,0001 kHz",   "10,00001 kHz",   "1,000001 kHz" };   // 1 000 001
    Array6 results_19999998   = { "9,999999 GHz",   "999,9999 MHz",   "99,99999 MHz",   "9,999999 MHz",   "999,9999 kHz",   "99,99999 kHz",   "9,999999 kHz" };   // 9 999 999
                                                                                                                                                
    Array6 results_20000002   = { "10,000001 GHz",  "1,0000001 GHz",  "100,00001 MHz",  "10,000001 MHz",  "1,0000001 MHz",  "100,00001 kHz",  "10,000001 kHz" };  // 10 000 001
    Array6 results_199999998  = { "99,999999 GHz",  "9,9999999 GHz",  "999,99999 MHz",  "99,999999 MHz",  "9,9999999 MHz",  "999,99999 kHz",  "99,999999 kHz" };  // 99 999 999
                                                  
    Array6 results_200000002  = { "100,000001 GHz", "10,0000001 GHz", "1,00000001 GHz", "100,000001 MHz", "10,0000001 MHz", "1,00000001 MHz", "100,000001 kHz" }; // 100 000 001
    Array6 results_1999999998 = { "999,999999 GHz", "99,9999999 GHz", "9,99999999 GHz", "999,999999 MHz", "99,9999999 MHz", "9,99999999 MHz", "999,999999 kHz" }; // 999 999 999

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

    Array6 *results[] =
    {
        &results_0, &results_2, &results_18,
        &results_20, &results_22, &results_198,
        &results_200, &results_202, &results_1998,
        &results_2000, &results_2002, &results_19998,
        &results_20000, &results_20002, &results_199998,
        &results_200002, &results_1999998,
        &results_2000002, &results_19999998,
        &results_20000002, &results_199999998,
        &results_200000002, &results_1999999998
    };

    StoreSettings(Channel::A);

    Channel::SetCurrent(Channel::A);
    Channel::A->mod.typeMeasure.value = TypeMeasure::Frequency;
    Channel::A->mod.modeFrequency.value = ModeFrequency::Frequency;

    for (int i = 0; counters[i] != (uint)-1; i++)
    {
        uint counter = counters[i];

        Array6 &array = *results[i];

        char **result = &array[0];

        for (uint8 time = 0; time < TimeMeasure::Count; time++)
        {
            Channel::A->mod.timeMeasure.value = time;

            ValueFPGA::Create(counter);

            char *value_str = GetMathValue();
            char *standard_str = *result;

            if (std::strcmp(value_str, standard_str) != 0)
            {
                FailExit();
            }

            result++;
        }
    }

    RestoreSettings(Channel::A);

    // Channel::B

    StoreSettings(Channel::B);

    Channel::SetCurrent(Channel::B);
    Channel::B->mod.typeMeasure.value = TypeMeasure::Frequency;
    Channel::B->mod.modeFrequency.value = ModeFrequency::Frequency;

    for (int i = 0; counters[i] != (uint)-1; i++)
    {
        uint counter = counters[i];

        Array6 &array = *results[i];

        char **result = &array[0];

        for (uint8 time = 0; time < TimeMeasure::Count; time++)
        {
            Channel::B->mod.timeMeasure.value = time;

            ValueFPGA::Create(counter);

            char *value_str = GetMathValue();
            char *standard_str = *result;

            if (std::strcmp(value_str, standard_str) != 0)
            {
                FailExit();
            }

            result++;
        }
    }

    RestoreSettings(Channel::B);

    TestChannelsCD();
}


static void Tests::Frequency::Frequency::TestChannelsCD()
{
    //                                              1 ms               10 ms              100 ms             1 s               10 s               100 s              1000s
    String results_0[TimeMeasure::Count]        = { "0 Hz",          "0 Hz",           "0 Hz",             "0 Hz",           "0 Hz",           "0 Hz",             "0 Hz" };
    String results_310[TimeMeasure::Count]      = { "9,92 MHz",      "992 kHz",        "99,2 kHz",         "9,92 kHz",       "992 Hz",         "99,2 Hz",          "9,92 Hz" };
    String results_312[TimeMeasure::Count]      = { "9,98 MHz",      "998 kHz",        "99,8 kHz",         "9,98 kHz",       "998 Hz",         "99,8 Hz",          "9,98 Hz" };
    String results_2000[TimeMeasure::Count]     = { "64,00 MHz",     "6,400 MHz",      "640,0 kHz",        "64,00 kHz",      "6,400 kHz",      "640,0 Hz",         "64,00 Hz" };
    String results_3126[TimeMeasure::Count]     = { "100,0 MHz",     "10,00 MHz",      "1,000 MHz",        "100,0 kHz",      "10,00 kHz",      "1,000 kHz",        "100,0 Hz" };
    String results_31250[TimeMeasure::Count]    = { "1,0000 GHz",    "100,00 MHz",     "10,000 MHz",       "1,0000 MHz",     "100,00 kHz",     "10,000 kHz",       "1,0000 kHz" };
    String results_312500[TimeMeasure::Count]   = { "10,0000 GHz",   "1,00000 GHz",    "100,000 MHz",      "10,0000 MHz",    "1,00000 MHz",    "100,000 kHz",      "10,0000 kHz" };
    String results_3125000[TimeMeasure::Count]  = { "100,0000 GHz",  "10,00000 GHz",   "1,000000 GHz",     "100,0000 MHz",   "10,00000 MHz",   "1,000000 MHz",     "100,0000 kHz" };
    String results_31250000[TimeMeasure::Count] = { "1,0000000 THz", "100,00000 GHz",  "10,000000 GHz",    "1,0000000 GHz",  "100,00000 MHz",  "10,000000 MHz",    "1,0000000 MHz" };

    String *results[] =
    {
        results_0,
        results_310,
        results_312,
        results_2000,
        results_3126,
        results_31250,
        results_312500,
        results_3125000,
        results_31250000
    };

    uint counters[] = {
        0,
        310,
        312,
        2000,
        3126,
        31250,
        312500,
        3125000,
        31250000,
        (uint)-1
    };

    StoreSettings(Channel::C);

    Channel::SetCurrent(Channel::C);
    Channel::C->mod.typeMeasure.value = TypeMeasure::Frequency;
    Channel::C->mod.modeFrequency.value = ModeFrequency::Frequency;

    for (int i = 0; counters[i] != (uint)-1; i++)
    {
        uint counter = counters[i];

        String *result = results[i];

        for (uint8 time = 0; time < TimeMeasure::Count; time++)
        {
            Channel::C->mod.timeMeasure.value = time;

            ValueFPGA::Create(counter);

            char *value_str = GetMathValue();
            char *standard_str = (*result).c_str();

            if (std::strcmp(value_str, standard_str) != 0)
            {
                FailExit();
            }

            result++;
        }
    }

    RestoreSettings(Channel::C);

    StoreSettings(Channel::D);

    Channel::SetCurrent(Channel::D);
    Channel::D->mod.typeMeasure.value = TypeMeasure::Frequency;
    Channel::D->mod.modeFrequency.value = ModeFrequency::Frequency;

    for (int i = 0; counters[i] != (uint)-1; i++)
    {
        uint counter = counters[i];

        String *result = results[i];

        for (uint8 time = 0; time < TimeMeasure::Count; time++)
        {
            Channel::D->mod.timeMeasure.value = time;

            ValueFPGA::Create(counter);

            char *value_str = GetMathValue();
            char *standard_str = (*result).c_str();

            if (std::strcmp(value_str, standard_str) != 0)
            {
                FailExit();
            }

            result++;
        }
    }

    RestoreSettings(Channel::D);

}


static void Tests::StoreSettings(Channel *channel)
{
    current = Channel::Current();
    type = channel->mod.typeMeasure.value;
    modeFrequency = channel->mod.modeFrequency.value;
    modePeriod = channel->mod.modePeriod.value;

    timeMeasure = channel->mod.timeMeasure.value;
    numberPeriods = channel->mod.numberPeriods.value;
    timeLabels = channel->mod.timeLabels.value;
}

static void Tests::RestoreSettings(Channel *channel)
{
    Channel::SetCurrent(current);
    channel->mod.typeMeasure.value = type;
    channel->mod.modeFrequency.value = modeFrequency;
    channel->mod.modePeriod.value = modePeriod;

    channel->mod.timeMeasure.value = timeMeasure;
    channel->mod.numberPeriods.value = numberPeriods;
    channel->mod.timeLabels.value = timeLabels;
}


static void Tests::Frequency::T_1::Test()
{
    struct ValuesStruct
    {
        ValuesStruct(pString s00, pString s01, pString s10, pString s11)
        {
            results[0][0].Set(s00);     // N == 1, L == 10-8
            results[0][1].Set(s01);     // N == 1, L == 10-3
            results[1][0].Set(s10);     // N == 100k, L == 10-8
            results[1][1].Set(s11);     // N == 100k, L == 10-3
        }

        char *c_str(int row, int col) { return results[row][col].c_str(); }

        String results[2][2];
    };

    struct ParametersStruct
    {
        NumberPeriods::E    numberPeriods;
        PeriodTimeLabels::E timeLabels;
    };

    ParametersStruct parameters[2][2] =
    {
        {{NumberPeriods::_1,    PeriodTimeLabels::T_8}, {NumberPeriods::_1,    PeriodTimeLabels::T_3}},
        {{NumberPeriods::_100K, PeriodTimeLabels::T_8}, {NumberPeriods::_100K, PeriodTimeLabels::T_3}}
    };

    ValuesStruct results_0         (ValueFPGA::UGO_DivNULL,
                                    ValueFPGA::UGO_DivNULL,
                                    ValueFPGA::UGO_DivNULL,
                                    ValueFPGA::UGO_DivNULL);
    ValuesStruct results_2         ("0,1 GHz",         "1 kHz",           "0,01 PHz",        "0,1 GHz");
    ValuesStruct results_20        ("10 MHz",          "0,10 kHz",        "1,0 THz",         "10 MHz");
    ValuesStruct results_200       ("1,00 MHz",        "10,0 Hz",         "100 GHz",         "1,00 MHz");
    ValuesStruct results_2000      ("100,0 kHz",       "1,000 Hz",        "10,00 GHz",       "100,0 kHz");
    ValuesStruct results_20000     ("10,000 kHz",      "100,00 mHz",      "1,0000 GHz",      "10,000 kHz");
    ValuesStruct results_200000    ("1,00000 kHz",     "10,0000 mHz",     "100,000 MHz",     "1,00000 kHz");
    ValuesStruct results_2000000   ("100,0000 Hz",     "1,000000 mHz",    "10,00000 MHz",    "100,0000 Hz");
    ValuesStruct results_20000000  ("10,000000 Hz",    "100,00000 uHz",   "1,0000000 MHz",   "10,000000 Hz");
    ValuesStruct results_200000000 ("1,00000000 Hz",   "10,0000000 uHz",  "100,000000 kHz",  "1,00000000 Hz");
    ValuesStruct results_2000000000("100,0000000 mHz", "1,000000000 uHz", "10,00000000 kHz", "100,0000000 mHz");

    struct TestStruct
    {
        uint          counter;
        ValuesStruct *values;
    };

    TestStruct structs[] =
    {
        {0,          &results_0},
        {2,          &results_2},
        {20,         &results_20},
        {200,        &results_200},
        {2000,       &results_2000},
        {20000,      &results_20000},
        {200000,     &results_200000},
        {2000000,    &results_2000000},
        {20000000,   &results_20000000},
        {200000000,  &results_200000000},
        {2000000000, &results_2000000000},
        {0, nullptr}
    };

    StoreSettings(Channel::A);

    Channel::SetCurrent(Channel::A);
    Channel::A->mod.typeMeasure.value = TypeMeasure::Frequency;
    Channel::A->mod.modeFrequency.value = ModeFrequency::T_1;

    for (int i = 0; structs[i].values != nullptr; i++)
    {
        TestStruct &s = structs[i];

        for (int row = 0; row < 2; row++)
        {
            for (int col = 0; col < 2; col++)
            {
                Channel::A->mod.numberPeriods.value = (uint8)parameters[row][col].numberPeriods;
                Channel::A->mod.timeLabels.value = (uint8)parameters[row][col].timeLabels;

                ValueFPGA::Create(s.counter);

                char *value_str = GetMathValue();
                char *standard_str = s.values->c_str(row, col);

                if (std::strcmp(value_str, standard_str) != 0)
                {
                    FailExit();
                }
            }
        }
    }

    RestoreSettings(Channel::A);
}


static void Tests::Period::Period::Test()
{
    struct ValuesStruct
    {
        ValuesStruct(pString s00, pString s01, pString s10, pString s11)
        {
            results[0][0].Set(s00);     // N == 1, L == 10-8
            results[0][1].Set(s01);     // N == 1, L == 10-3
            results[1][0].Set(s10);     // N == 100k, L == 10-8
            results[1][1].Set(s11);     // N == 100k, L == 10-3
        }

        char *c_str(int row, int col) { return results[row][col].c_str(); }

        String results[2][2];
    };

    struct ParametersStruct
    {
        NumberPeriods::E    numberPeriods;
        PeriodTimeLabels::E timeLabels;
    };

    ParametersStruct parameters[2][2] =
    {
        {{NumberPeriods::_1,    PeriodTimeLabels::T_8}, {NumberPeriods::_1,    PeriodTimeLabels::T_3}},
        {{NumberPeriods::_100K, PeriodTimeLabels::T_8}, {NumberPeriods::_100K, PeriodTimeLabels::T_3}}
    };

    ValuesStruct results_0         ("0 s",           "0 s",            "0 s",            "0 s");
    ValuesStruct results_2         ("0,01 us",       "1 ms",           "0,1 ps",         "0,01 us");
    ValuesStruct results_20        ("0,10 us",       "10 ms",          "1,0 ps",         "0,10 us");
    ValuesStruct results_200       ("1,00 us",       "100 ms",         "10,0 ps",        "1,00 us");
    ValuesStruct results_2000      ("10,00 us",      "1,000 s",        "100,0 ps",       "10,00 us");
    ValuesStruct results_20000     ("100,00 us",     "10,000 s",       "1,0000 ns",      "100,00 us");
    ValuesStruct results_200000    ("1,00000 ms",    "100,000 s",      "10,0000 ns",     "1,00000 ms");
    ValuesStruct results_2000000   ("10,00000 ms",   "1,000000 ks",    "100,0000 ns",    "10,00000 ms");
    ValuesStruct results_20000000  ("100,00000 ms",  "10,000000 ks",   "1,0000000 us",   "100,00000 ms");
    ValuesStruct results_200000000 ("1,00000000 s",  "100,000000 ks",  "10,0000000 us",  "1,00000000 s");
    ValuesStruct results_2000000000("10,00000000 s", "1,000000000 Ms", "100,0000000 us", "10,00000000 s");

    struct TestStruct
    {
        uint          counter;
        ValuesStruct *values;
    };

    TestStruct structs[] =
    {
        {0,          &results_0},
        {2,          &results_2},
        {20,         &results_20},
        {200,        &results_200},
        {2000,       &results_2000},
        {20000,      &results_20000},
        {200000,     &results_200000},
        {2000000,    &results_2000000},
        {20000000,   &results_20000000},
        {200000000,  &results_200000000},
        {2000000000, &results_2000000000},
        {0, nullptr}
    };

    StoreSettings(Channel::A);

    Channel::SetCurrent(Channel::A);
    Channel::A->mod.typeMeasure.value = TypeMeasure::Period;
    Channel::A->mod.modePeriod.value = ModePeriod::Period;

    for (int i = 0; structs[i].values != nullptr; i++)
    {
        TestStruct &s = structs[i];

        for (int row = 0; row < 2; row++)
        {
            for (int col = 0; col < 2; col++)
            {
                Channel::A->mod.numberPeriods.value = (uint8)parameters[row][col].numberPeriods;
                Channel::A->mod.timeLabels.value = (uint8)parameters[row][col].timeLabels;

                ValueFPGA::Create(s.counter);

                char *value_str = GetMathValue();
                char *standard_str = s.values->c_str(row, col);

                if (std::strcmp(value_str, standard_str) != 0)
                {
                    FailExit();
                }
            }
        }
    }

    RestoreSettings(Channel::A);
}


static void Tests::Period::F_1::Test()
{
    String results_0[TimeMeasure::Count] = {
        ValueFPGA::UGO_DivNULL,
        ValueFPGA::UGO_DivNULL,
        ValueFPGA::UGO_DivNULL,
        ValueFPGA::UGO_DivNULL,
        ValueFPGA::UGO_DivNULL,
        ValueFPGA::UGO_DivNULL,
        ValueFPGA::UGO_DivNULL
    };
    String results_2[TimeMeasure::Count]          = { "1 ms",           "0,01 s",         "0,1 s",          "1 s",            "0,01 ks",        "0,1 ks",         "1 ks" };      // 1
    String results_18[TimeMeasure::Count]         = { "0,1 ms",         "1 ms",           "0,01 s",         "0,1 s",          "1 s",            "0,01 ks",        "0,1 ks" };       // 9
                                                  
    String results_20[TimeMeasure::Count]         = { "0,10 ms",        "1,0 ms",         "10 ms",          "0,10 s",         "1,0 s",          "10 s",           "0,10 ks" };       // 10
    String results_22[TimeMeasure::Count]         = { "90 us",          "0,90 ms",        "9,0 ms",         "90 ms",          "0,90 s",         "9,0 s",          "90 s" };       // 11
    String results_198[TimeMeasure::Count]        = { "10 us",          "0,10 ms",        "1,0 ms",         "10 ms",          "0,10 s",         "1,0 s",          "10 s" };      // 99
                                                  
      
    String results_199999980[TimeMeasure::Count]  = { "10,000001 ps",   "100,00001 ps",   "1,0000001 ns",   "10,000001 ns",   "100,00001 ns",   "1,0000001 us",   "10,000001 us" }; // 100 000 001
    String results_199999990[TimeMeasure::Count]  = { "10,000000 ps",   "100,00000 ps",   "1,0000000 ns",   "10,000000 ns",   "100,00000 ns",   "1,0000000 us",   "10,000000 us" }; // 100 000 001
    String results_199999998[TimeMeasure::Count]  = { "10,000000 ps",   "100,00000 ps",   "1,0000000 ns",   "10,000000 ns",   "100,00000 ns",   "1,0000000 us",   "10,000000 us" }; // 100 000 001
    String results_200000000[TimeMeasure::Count]  = { "10,0000000 ps",  "100,000000 ps",  "1,00000000 ns",  "10,0000000 ns",  "100,000000 ns",  "1,00000000 us",  "10,0000000 us" }; // 100 000 001
    String results_200000002[TimeMeasure::Count]  = { "9,99999900 ps",  "99,9999990 ps",  "999,999990 ps",  "9,99999990 ns",  "99,9999990 ns",  "999,999990 ns",  "9,99999990 us" }; // 100 000 001
 
    String results_1999999998[TimeMeasure::Count] = { "1,00000000 ps",  "10,0000000 ps",  "100,000000 ps",  "1,00000000 ns",  "10,0000000 ns",  "100,000000 ns",  "1,00000000 us" }; // 999 999 999

    uint counters[] = {
        0, 2, 18,
        20, 22, 198,
        199999980, 199999990, 199999998,
        200000000, 200000002, 1999999998,
        (uint)-1
    };

    String *results[] =
    {
        results_0, results_2, results_18,
        results_20, results_22, results_198,
        results_199999980, results_199999990, results_199999998,
        results_200000000, results_200000002, results_1999999998
    };


    StoreSettings(Channel::A);

    Channel::SetCurrent(Channel::A);
    Channel::A->mod.typeMeasure.value = TypeMeasure::Period;
    Channel::A->mod.modePeriod.value = ModePeriod::F_1;

    for (int i = 0; counters[i] != (uint)-1; i++)
    {
        uint counter = counters[i];

        String *result = results[i];

        for (uint8 time = 0; time < TimeMeasure::Count; time++)
        {
            Channel::A->mod.timeMeasure.value = time;

            ValueFPGA::Create(counter);

            char *value_str = GetMathValue();
            char *standard_str = (*result).c_str();

            if (std::strcmp(value_str, standard_str) != 0)
            {
                FailExit();
            }

            result++;
        }
    }

    RestoreSettings(Channel::A);
}
