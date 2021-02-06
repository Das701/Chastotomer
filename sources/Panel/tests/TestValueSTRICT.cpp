#include "defines.h"
#include "Tests.h"
#include "Utils/ValueSTRICT.h"
#include <cstdlib>


#define NUM_CYCLES  1000


namespace Tests
{
    static void AllTests(ValueSTRICT value);

    static void TestSub(ValueSTRICT value);
}


void Tests::ValuesSTRICT()
{
    for (int i = 0; i < NUM_CYCLES; i++)
    {
        double value = (double)(std::rand() * std::rand()) / (double)std::rand();

        AllTests(ValueSTRICT(value));
    }
}


void Tests::AllTests(ValueSTRICT value)
{
    TestSub(value);
}


void Tests::TestSub(ValueSTRICT one)
{
    ValueSTRICT pattern = one;

    one.Abs();

    ValueSTRICT two = one;

    two.Sub(one);

    if (!two.IsZero())
    {
        FailExit();
    }

    one.SetSign(1);
    two.SetSign(-1);

    one.Sub(two);

    if (one != pattern * 2)
    {
        FailExit();
    }
}
