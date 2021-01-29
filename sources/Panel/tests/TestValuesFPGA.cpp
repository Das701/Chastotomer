#include "defines.h"
#include "Tests.h"
#include "Calculate/MathFPGA.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Menu/Pages/Modes/Modes.h"
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


static void Tests::Frequency::Frequency::Test()
{
    // Channel::A

    //                                                1 ms              10 ms             100 ms            1 s               10 s              100 s             1000s
    String results_0[TimeMeasure::Count]          = { "0 Hz",           "0 Hz",           "0 Hz",           "0 Hz",           "0 Hz",           "0 Hz",           "0 Hz"};         // 0
    String results_2[TimeMeasure::Count]          = { "1 kHz",          "0,1 kHz",        "0,01 kHz",       "1 Hz",           "0,1 Hz",         "0,01 Hz",        "1 mHz" };      // 1
    String results_18[TimeMeasure::Count]         = { "9 kHz",          "0,9 kHz",        "0,09 kHz",       "9 Hz",           "0,9 Hz",         "0,09 Hz",        "9 mHz"};       // 9
                                                                                                                                                
    String results_20[TimeMeasure::Count]         = { "10 kHz",         "1,0 kHz",        "0,10 kHz",       "10 Hz",          "1,0 Hz",         "0,10 Hz",        "10 mHz"};       // 10
    String results_22[TimeMeasure::Count]         = { "11 kHz",         "1,1 kHz",        "0,11 kHz",       "11 Hz",          "1,1 Hz",         "0,11 Hz",        "11 mHz"};       // 11
    String results_198[TimeMeasure::Count]        = { "99 kHz",         "9,9 kHz",        "0,99 kHz",       "99 Hz",          "9,9 Hz",         "0,99 Hz",        "99 mHz" };      // 99
                                                                                                                                                
    String results_200[TimeMeasure::Count]        = { "100 kHz",        "10,0 kHz",       "1,00 kHz",       "100 Hz",         "10,0 Hz",        "1,00 Hz",        "100 mHz" };      // 100
    String results_202[TimeMeasure::Count]        = { "101 kHz",        "10,1 kHz",       "1,01 kHz",       "101 Hz",         "10,1 Hz",        "1,01 Hz",        "101 mHz" };      // 101
    String results_1998[TimeMeasure::Count]       = { "999 kHz",        "99,9 kHz",       "9,99 kHz",       "999 Hz",         "99,9 Hz",        "9,99 Hz",        "999 mHz"};       // 999
                                                                                                                                                
    String results_2000[TimeMeasure::Count]       = { "1,000 MHz",      "100,0 kHz",      "10,00 kHz",      "1,000 kHz",      "100,0 Hz",       "10,00 Hz",       "1,000 Hz" };     // 1 000
    String results_2002[TimeMeasure::Count]       = { "1,001 MHz",      "100,1 kHz",      "10,01 kHz",      "1,001 kHz",      "100,1 Hz",       "10,01 Hz",       "1,001 Hz" };     // 1 001
    String results_19998[TimeMeasure::Count]      = { "9,999 MHz",      "999,9 kHz",      "99,99 kHz",      "9,999 kHz",      "999,9 Hz",       "99,99 Hz",       "9,999 Hz" };     // 9 999
                                                                                                                                                
    String results_20000[TimeMeasure::Count]      = { "10,000 MHz",     "1,0000 MHz",     "100,00 kHz",     "10,000 kHz",     "1,0000 kHz",     "100,00 Hz",      "10,000 Hz" };     // 10 000
    String results_20002[TimeMeasure::Count]      = { "10,001 MHz",     "1,0001 MHz",     "100,01 kHz",     "10,001 kHz",     "1,0001 kHz",     "100,01 Hz",      "10,001 Hz" };     // 10 001
    String results_199998[TimeMeasure::Count]     = { "99,999 MHz",     "9,9999 MHz",     "999,99 kHz",     "99,999 kHz",     "9,9999 kHz",     "999,99 Hz",      "99,999 Hz" };     // 99 999
                                                                                                                                                
    String results_200002[TimeMeasure::Count]     = { "100,001 MHz",    "10,0001 MHz",    "1,00001 MHz",    "100,001 kHz",    "10,0001 kHz",    "1,00001 kHz",    "100,001 Hz" };    // 100 001
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

    StoreSettings(Channel::A);

    Channel::SetCurrent(Channel::A);
    Channel::A->mod.typeMeasure.value = TypeMeasure::Frequency;
    Channel::A->mod.modeFrequency.value = ModeFrequency::Frequency;

    for (int i = 0; counters[i] != (uint)-1; i++)
    {
        uint counter = counters[i];

        String *result = results[i];

        for (uint8 time = 0; time < TimeMeasure::Count; time++)
        {
            Channel::A->mod.timeMeasure.value = time;

            MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::MainCounters, counter, 0);

            char *value_str = MathFPGA::Measure::valueFPGA->value.c_str();
            char *standard_str = (*result).c_str();

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

        String *result = results[i];

        for (uint8 time = 0; time < TimeMeasure::Count; time++)
        {
            Channel::B->mod.timeMeasure.value = time;

            MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::MainCounters, counter, 0);

            char *value_str = MathFPGA::Measure::valueFPGA->value.c_str();
            char *standard_str = (*result).c_str();

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

            MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::MainCounters, counter, 0);

            char *value_str = MathFPGA::Measure::valueFPGA->value.c_str();
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

            MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::MainCounters, counter, 0);

            char *value_str = MathFPGA::Measure::valueFPGA->value.c_str();
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
            results[0][0].Set(TypeConversionString::None, s00);     // N == 1, L == 10-8
            results[0][1].Set(TypeConversionString::None, s01);     // N == 1, L == 10-3
            results[1][0].Set(TypeConversionString::None, s10);     // N == 100k, L == 10-8
            results[1][1].Set(TypeConversionString::None, s11);     // N == 100k, L == 10-3
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

    ValuesStruct results_0         (MathFPGA::Data::UGO_DivNULL,
                                    MathFPGA::Data::UGO_DivNULL,
                                    MathFPGA::Data::UGO_DivNULL,
                                    MathFPGA::Data::UGO_DivNULL);
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

                MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::MainCounters, s.counter, 0);

                char *value_str = MathFPGA::Measure::valueFPGA->value.c_str();
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
            results[0][0].Set(TypeConversionString::None, s00);     // N == 1, L == 10-8
            results[0][1].Set(TypeConversionString::None, s01);     // N == 1, L == 10-3
            results[1][0].Set(TypeConversionString::None, s10);     // N == 100k, L == 10-8
            results[1][1].Set(TypeConversionString::None, s11);     // N == 100k, L == 10-3
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

                MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::MainCounters, s.counter, 0);

                char *value_str = MathFPGA::Measure::valueFPGA->value.c_str();
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
    //                                                1 ms              10 ms             100 ms            1 s               10 s              100 s             1000s
    String results_0[TimeMeasure::Count] = {
        MathFPGA::Data::UGO_DivNULL,
        MathFPGA::Data::UGO_DivNULL,
        MathFPGA::Data::UGO_DivNULL,
        MathFPGA::Data::UGO_DivNULL,
        MathFPGA::Data::UGO_DivNULL,
        MathFPGA::Data::UGO_DivNULL,
        MathFPGA::Data::UGO_DivNULL };
    String results_2[TimeMeasure::Count]          = { "1,0 ms",         "10 ms",           "0,10 s",       "1,0 s",           "10 s",         "0,10 ks",        "1,0 ks" };      // 1
    String results_4[TimeMeasure::Count]          = { "0,50 ms",        "5,0 ms",          "50 ms",        "0,50 s",          "5,0 s",        "50 s",           "0,50 ks" };
    String results_6[TimeMeasure::Count]          = { "0,33 ms",        "3,3 ms",          "33 ms",        "0,33 s",          "3,3 s",        "33 s",           "0,33 ks" };
    String results_8[TimeMeasure::Count]          = { "0,25 ms",        "2,5 ms",          "25 ms",        "0,25 s",          "2,5 s",        "25 s",           "0,25 ks" };
    String results_10[TimeMeasure::Count]         = { "0,20 ms",        "2,0 ms",          "20 ms",        "0,20 s",          "2,0 s",        "20 s",           "0,20 ks" };
/*6*/  String results_12[TimeMeasure::Count]       = { "0,16 ms",        "1,6 ms",          "16 ms",        "0,16 s",          "1,6 s",        "16 s",           "0,16 ks" };
/*7*/  String results_14[TimeMeasure::Count]       = { "0,14 ms",        "1,4 ms",          "14 ms",        "0,14 s",          "1,4 s",        "14 s",           "0,14 ks" };
/*8*/  String results_16[TimeMeasure::Count]       = { "0,12 ms",        "1,2 ms",          "12 ms",        "0,12 s",          "1,2 s",        "12 s",           "0,12 ks" };
/*9*/  String results_18[TimeMeasure::Count]       = { "0,11 ms",        "1,1 ms",          "11 ms",        "0,11 s",          "1,1 s",        "11 s",           "0,11 ks" };
/*10*/ String results_20[TimeMeasure::Count]       = { "100 us",        "1,00 ms",          "10,0 ms",         "100 ms",         "1,00 s",         "10,0 s",          "100 s" };       // 10
//    String results_22[TimeMeasure::Count]         = { "90,9 us",          "909 us",         "9,09 ms",        "90,9 ms",          "909 ms",        "9,09 s",         "90,9 s" };       // 10
//    String results_24[TimeMeasure::Count]         = { "83 us",          "0,83 ms",         "8,3 ms",        "83 ms",          "0,83 s",        "8,3 s",         "83 s" };       // 10
//    String results_26[TimeMeasure::Count]         = { "76 us",          "0,76 ms",         "7,6 ms",        "76 ms",          "0,76 s",        "7,6 s",         "76 s" };
//    String results_30[TimeMeasure::Count]         = { "66 us",          "0,66 ms",         "6,6 ms",        "66 ms",          "0,66 s",        "6,6 s",         "66 s" };       // 10
//    String results_180[TimeMeasure::Count]        = { "11 us",          "0,11 ms",         "1,1 ms",        "11 ms",          "0,11 s",        "1,1 s",         "11 s" };
    String results_190[TimeMeasure::Count]        = { "10,5 us",          "105 us",        "1,05 ms",       "10,5 ms",        "105 ms",        "1,05 s",        "10,5 s" };
    String results_194[TimeMeasure::Count]        = { "10,3 us",          "103 us",         "1,03 ms",        "10,3 ms",          "103 ms",        "1,03 s",         "10,3 s" };
    String results_196[TimeMeasure::Count]        = { "10,2 us",          "102 us",         "1,02 ms",        "10,2 ms",          "102 ms",        "1,02 s",         "10,2 s" };
    String results_198[TimeMeasure::Count]        = { "10,1 us",          "101 us",         "1,01 ms",        "10,1 ms",          "101 ms",        "1,01 s",         "10,1 s" };
    String results_200[TimeMeasure::Count]        = { "10,00 us",        "100,0 us",          "1,000 ms",       "10,00 ms",        "100,0 ms",        "1,000 s",        "10,00 s" };       // 10
    String results_1998[TimeMeasure::Count]       = { "1,001 us",        "10,01 us",         "100,1 us",        "1,001 ms",        "10,01 ms",       "100,1 ms",        "1,001 s" };
    String results_2000[TimeMeasure::Count]       = { "1,0000 us",       "10,000 us",        "100,00 us",      "1,0000 ms",       "10,000 ms",      "100,00 ms",      "1,0000 s" };

    String results_2002[TimeMeasure::Count]       = { "999,00 ns",       "9,9900 us",        "99,900 us",      "999,00 us",       "9,9900 ms",      "99,900 ms",      "999,00 ms" };
    String results_2004[TimeMeasure::Count]       = { "998,00 ns",       "9,9800 us",        "99,800 us",      "998,00 us",       "9,9800 ms",      "99,800 ms",      "998,00 ms" };

    String results_200000[TimeMeasure::Count]     = { "10,00000 ns",     "100,0000 ns",      "1,000000 us",    "10,00000 us",     "100,0000 us",    "1,000000 ms",    "10,00000 ms" };
    String results_200002[TimeMeasure::Count]     = { "9,999900 ns",     "99,99900 ns",      "999,9900 ns",    "9,999900 us",     "99,99900 us",    "999,9900 us",    "9,999900 ms" };
    String results_200004[TimeMeasure::Count]     = { "9,999800 ns",     "99,99800 ns",      "999,9800 ns",    "9,999800 us",     "99,99800 us",    "999,9800 us",    "9,999800 ms" };
    String results_200006[TimeMeasure::Count]     = { "9,999700 ns",     "99,99700 ns",      "999,9700 ns",    "9,999700 us",     "99,99700 us",    "999,9700 us",    "9,999700 ms" };
    String results_200008[TimeMeasure::Count]     = { "9,999600 ns",     "99,99600 ns",      "999,9600 ns",    "9,999600 us",     "99,99600 us",    "999,9600 us",    "9,999600 ms" };

    String *results[] =
    {
        results_0,
        results_2,
        results_4,
        results_6,
        results_8,
        results_10,
        results_12,
        results_14,
        results_16,
        results_18,
        results_20,
//        results_22,
//        results_24,
//        results_26,
//        results_30,
//        results_180,
        results_190,
        results_194,
        results_196,
        results_198,
        results_200,
        results_1998,
        results_2000,
        results_2002,
        results_2004,
        results_200000,
        results_200002,
        results_200004,
        results_200006,
        results_200008
    };

    uint counters[] = {
        0,
        2,
        4,
        6,
        8,
        10,
        12,
        14,
        16,
        18,
        20,
//        22,
//        24,
//        26,
//        30,
//        180,
        190,
        194,
        196,
        198,
        200,
        1998,
        2000,
        2002,
        2004,
        200000,
        200002,
        200004,
        200006,
        200008,
        (uint)-1
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

            MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::MainCounters, counter, 0);

            char *value_str = MathFPGA::Measure::valueFPGA->value.c_str();
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
