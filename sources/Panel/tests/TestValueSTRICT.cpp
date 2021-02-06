#include "defines.h"
#include "Tests.h"
#include "Utils/ValueSTRICT.h"
#include <cstdlib>


#define NUM_CYCLES  1000


namespace Tests
{
    static void AllTests(ValueSTRICT value);

    static void TestEqualSub(const ValueSTRICT value);
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
    TestEqualSub(value);
}


static void Tests::TestEqualSub(const ValueSTRICT value)
{
    ValueSTRICT one = value;
    ValueSTRICT two = value;


    ValueSTRICT result(0);

    {
        one.SetSign(1);
        two.SetSign(1);

        result = one;
        result.Sub(two);
        if (!result.IsZero())
        {
            FailExit();
        }
    }

    {
        one.SetSign(1);
        two.SetSign(-1);

        result = one;
        result.Sub(two);

        if (result != one * 2)
        {
            FailExit();
        }
    }

    {
        one.SetSign(-1);
        two.SetSign(1);

        result = one;
        result.Sub(two);

        if (result != one * 2)
        {
            FailExit();
        }
    }

    {
        one.SetSign(-1);
        two.SetSign(-1);

        result = one;
        result.Sub(two);

        if (!result.IsZero())
        {
            FailExit();
        }
    }
}
