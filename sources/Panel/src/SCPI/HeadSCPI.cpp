#include "defines.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"


// *IDN?
static pCHAR FuncIDN(pCHAR);
static bool TestIDN();
static void HintIDN(String *);

// *RST
static pCHAR FuncReset(pCHAR);
static bool TestReset();
static void HintReset(String *);

// :HELP
static pCHAR FuncHelp(pCHAR);
static bool TestHelp();
static void HintHelp(String *);
static void ProcessHelp(const StructSCPI *strct, String message);

// :TEST
static pCHAR FuncTest(pCHAR);
static bool TestTest();
static void HintTest(String *);

// :CALIBRATOR:MODE
static pCHAR FuncCalibratorMode(pCHAR);
static bool TestCalibratorMode();
static void HintCalibratorMode(String *);

// :MEMORY:LENGTH
static pCHAR FuncMemoryLength(pCHAR);
static bool TestMemoryLength();
static void HintMemoryLength(String *);

// :MEMORY:SAVE
static pCHAR FuncMemorySave(pCHAR);
static bool TestMemorySave();
static void HintMemorySave(String *);

// :RUN
static pCHAR FuncRun(pCHAR);
static bool TestRun();
static void HintRun(String *);

// :STOP
static pCHAR FuncStop(pCHAR);
static bool TestStop();
static void HintStop(String *);

// :AUTOSEARCH
static pCHAR FuncAutoSearch(pCHAR);
static bool TestAutoSearch();
static void HintAutoSearch(String *);


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",            FuncIDN,            TestIDN,            "ID request",                       HintIDN),
    SCPI_LEAF("*RST",             FuncReset,          TestReset,          "Reset settings to default values", HintReset),
    SCPI_LEAF(":HELP",            FuncHelp,           TestHelp,           "Output of this help",              HintHelp),
    SCPI_LEAF(":TEST",            FuncTest,           TestTest,           "Run all tests",                    HintTest),
    SCPI_LEAF(":CALIBRATOR:MODE", FuncCalibratorMode, TestCalibratorMode, "",                                 HintCalibratorMode),
    SCPI_LEAF(":MEMORY:LENGTH",   FuncMemoryLength,   TestMemoryLength,   "",                                 HintMemoryLength),
    SCPI_LEAF(":MEMORY:SAVE",     FuncMemorySave,     TestMemorySave,     "",                                 HintMemorySave),
    SCPI_LEAF(":RUN",             FuncRun,            TestRun,            "",                                 HintRun),
    SCPI_LEAF(":STOP",            FuncStop,           TestStop,           "",                                 HintStop),
    SCPI_LEAF(":AUTOSEARCH",      FuncAutoSearch,     TestAutoSearch,     "",                                 HintAutoSearch),
    SCPI_EMPTY()
};


static pCHAR FuncIDN(pCHAR buffer)
{
    SCPI_PROLOG(buffer)

    SCPI::SendAnswer(String("OAO MNIPI, CH3-96/2, Cherem V1.2").c_str());

    SCPI_EPILOG(buffer)
}


static void HintIDN(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static pCHAR FuncReset(pCHAR buffer)
{
    SCPI_PROLOG(buffer)
        
    SCPI_EPILOG(buffer)
}


static void HintReset(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static pCHAR FuncHelp(pCHAR buffer)
{
    SCPI_PROLOG(buffer);
    
    String message;

    ProcessHelp(SCPI::head, message);

    SCPI_EPILOG(buffer);
}


static void HintHelp(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static pCHAR FuncTest(pCHAR buffer)
{
    static const char *const modes[2] =
    {
        " FULL",
        " FAST"
    };

    for (int i = 0; i < 2; i++)
    {
        pCHAR end = SCPI::BeginWith(buffer, modes[i]);
        if (end)
        {
            SCPI_PROLOG(end)

           SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static pCHAR FuncAutoSearch(pCHAR buffer)
{
    return buffer;
}


static pCHAR FuncCalibratorMode(pCHAR buffer)
{
    return 0;
}


static pCHAR FuncMemoryLength(pCHAR buffer)
{
    return 0;
}


static pCHAR FuncMemorySave(pCHAR buffer)
{
    SCPI_PROLOG(buffer);

    SCPI_EPILOG(buffer);
}


static pCHAR FuncRun(pCHAR buffer)
{
    SCPI_PROLOG(buffer);

    SCPI_EPILOG(buffer);
}


static pCHAR FuncStop(pCHAR buffer)
{
    SCPI_PROLOG(buffer);

    SCPI_EPILOG(buffer);
}


static void HintAutoSearch(String *)
{

}


static void HintCalibratorMode(String *)
{

}


static void HintMemoryLength(String *)
{

}


static void HintMemorySave(String *)
{

}


static void HintRun(String *)
{

}


static void HintStop(String *)
{

}


static void HintTest(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static bool TestAutoSearch()
{
    return false;
}


static bool TestCalibratorMode()
{
    return false;
}


static bool TestMemoryLength()
{
    return false;
}


static bool TestMemorySave()
{
    return false;
}


static bool TestRun()
{
    return false;
}


static bool TestStop()
{
    return false;
}


static bool TestIDN()
{
    return true;
}


static bool TestReset()
{
    return true;
}


static bool TestHelp()
{
    return true;
}


static bool TestTest()
{
    return true;
}


static void ProcessHelp(const StructSCPI *strct, String msg)
{
    while(!strct->IsEmpty())
    {
        if(strct->IsNode())
        {
            String message(msg);
            message.Append(strct->key);
            ProcessHelp(strct->strct, message);
        }
        else if(strct->IsLeaf())
        {
            String message(msg); //-V820
            message.Append(strct->key);
            SCPI::SendAnswer(strct->hint);
            strct->funcHint(&message);
            SCPI::SendAnswer("");
        }

        strct++;
    }
}
