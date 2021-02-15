#include "defines.h"
#include "Hardware/VCP.h"
#include "Menu/Pages/Channels/Channels.h"
#include "SCPI/SCPI.h"
#include "SCPI/StringUtils.h"
#include "Utils/Buffer.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include <cstring>


// ����������� ������� ��������� ������� �������� StructSCPI.
// � ������ ��������� ���������� ���������� ����� �������, �������������� �� ��������� ������������ ��������.
// � ������ ����������� ���������� - ���������� nullptr. ��� ������ ��������� � *error
static pchar Process(pchar buffer, const StructSCPI structs[]); //-V2504

// ��������� ���� ������ node
static pchar ProcessNode(pchar begin, const StructSCPI *node);

// ��������� ����� node
static pchar ProcessLeaf(pchar begin, const StructSCPI *node);

// ���������� true, ���� ������ �������� ������� ������� - ������������ ��� '*'
static bool IsBeginCommand(const char &symbol);

// ������� ������������ ������� �� ������ ������
static void RemoveBadSymbolsFromBegin();

// ������� ������������������ ������������ �� ������ ������ �� ���������� ����������
static bool RemoveSeparatorsSequenceFromBegin();

// ������� ��� ������� �� ������� �����������
static bool RemoveSymbolsBeforeSeparator();

static String inputBuffer;

static String badSymbols;


void SCPI::AppendNewData(pchar buffer, int size)
{
    inputBuffer.Append(buffer, size);

    ::SU::ToUpper(inputBuffer.c_str());

    RemoveBadSymbolsFromBegin();

    if (inputBuffer.Size() == 0)
    {
        Answer::SendBadSymbols();
    }
}


void SCPI::Update()
{
    RemoveBadSymbolsFromBegin();

    if(inputBuffer.Size() == 0)
    {
        Answer::SendBadSymbols();
        return;
    }

    pchar end = Process(inputBuffer.c_str(), head);

    if(end)
    {
        inputBuffer.RemoveFromBegin(static_cast<int>(end - inputBuffer.c_str()));
    }
}


static pchar Process(pchar buffer, const StructSCPI strct[]) //-V2504
{
    while (!strct->IsEmpty())
    {
        pchar end = SCPI::SU::BeginWith(buffer, strct->key);

        if (end)
        {
            if (strct->IsNode())
            {
                return ProcessNode(end, strct);
            }
            else if (strct->IsLeaf()) //-V2516
            {
                return ProcessLeaf(end, strct);
            }
        }

        strct++;
    }

    badSymbols.Append(*buffer);         // ��������� ��� ����� � strct � �� ����� �� ������ ������������. ������� �������� ��������� ����������� � ����������� ��������

    return buffer + 1;
}


static pchar ProcessNode(pchar begin, const StructSCPI *node)
{
    return Process(begin, node->strct);
}


static pchar ProcessLeaf(pchar begin, const StructSCPI *node)
{
    if (*begin == '\0')                     // ������������ �� ����, ��� ������ ��������� ������� �� ������
    {
        return nullptr;
    }

    pchar result = node->func(begin);

    if (result)
    {
        return result;
    }

    badSymbols.Append(*begin);

    return begin + 1;
}


static void RemoveBadSymbolsFromBegin()
{
    while (RemoveSymbolsBeforeSeparator() || RemoveSeparatorsSequenceFromBegin())  { }
}


static bool RemoveSymbolsBeforeSeparator()
{
    bool result = false;

    while ((inputBuffer.Size() != 0) && !IsBeginCommand(inputBuffer[0]))
    {
        badSymbols.Append(inputBuffer[0]);
        inputBuffer.RemoveFromBegin(1);
        result = true;
    }

    return result;
}


static bool RemoveSeparatorsSequenceFromBegin()
{
    bool result = false;

    while (inputBuffer.Size() > 1 && IsBeginCommand(inputBuffer[0]) && IsBeginCommand(inputBuffer[1]))
    {
        badSymbols.Append(inputBuffer[0]);
        inputBuffer.RemoveFromBegin(1);
        result = true;
    }

    return result;
}


void SCPI::SendAnswer(pchar message)
{
    while (*message == ' ')
    {
        message++;
    }

    if(message[std::strlen(message) - 1] != 0x0D)
    {
        String msg(message);
        msg.Append(0x0D);
        VCP::SendStringAsynch(msg.c_str());
    }
    else
    {
        VCP::SendStringAsynch(message);
    }
}


void SCPI::SendData(pchar message)
{
    VCP::SendStringAsynch(message);
}


void SCPI::SendData(const String &message)
{
    VCP::SendStringAsynch(message.c_str());
}


static bool IsBeginCommand(const char &symbol)
{
    return (symbol == SCPI::SEPARATOR) || (symbol == '*');
}


void SCPI::ProcessHint(String *message, pchar const *names)
{
    message->Append(" {");
    for(int i = 0; i < names[i][0] != 0; i++)
    {
        message->Append(names[i]);
        message->Append(" |");
    }
    message->RemoveFromEnd();
    message->Append('}');
    SCPI::SendAnswer(message->c_str());
}


void SCPI::SendMeasure(const String &str)
{
    String message;

    for (int i = 0; i < str.Size(); i++)
    {
        char symbol = str[i];

        if (static_cast<uint8>(symbol) == 0xa8)     // ���� ���������� ������ ����
        {
            continue;
        }

        if (symbol == '\x10')     { symbol = ' '; }
        else if (symbol == '�')
        {
            if (str[i + 1] == '�') { message.Append('u'); i++; }
            else                   { message.Append('m');      }
            continue;
        }
        else if (symbol == '�')    { symbol = 'n'; }
        else if (symbol == '�')    { symbol = 's'; }
        else if (symbol == '�')    { symbol = 'V'; }
        else if (symbol == '�')    { symbol = 'A'; }
        else if (symbol == '�')    { symbol = 'M'; }
        else if (symbol == '�')    { symbol = 'k'; }
        else if ((symbol == '�') && (str[i + 1] == '�')) //-V2516
        {
            message.Append('H');  message.Append('z');  i++; continue;
        }

        message.Append(symbol);
    }

    SendAnswer(message.c_str());
}


void SCPI::Answer::SendBadSymbols()
{
    if (badSymbols.Size())
    {
        String message("!!! ERROR !!! Invalid sequency : %s", badSymbols.c_str());
        SCPI::SendAnswer(message.c_str());
        badSymbols.Free();
    }
}


void SCPI::Answer::CurrentChannelHasNotParameter()
{
    SendAnswer("!!! ERROR !!! Current channel does has not this parameter");
}


void SCPI::Answer::InvalidParameter()
{
    SendAnswer("!!! ERROR !!! Invalid parameter");
}


void SCPI::Answer::ThisModeCannotBeSetForTheCurrentChannel()
{
    SendAnswer("!!! ERROR !!! This mode cannot be set for the current channel");
}


// ����� ������� ��� �������� ������ �� ��������� ����� �������
static void AnswerInput(const pchar choice[], uint8 value)
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        SCPI::SendAnswer(choice[value]);
    }
    else
    {
        SCPI::Answer::CurrentChannelHasNotParameter();
    }
}


pchar SCPI::ProcessSimpleParameter(pchar buffer, const pchar choice[], Switch *const sw)
{
    SCPI_REQUEST(AnswerInput(choice, sw->Value()));

    SCPI_PROCESS_ARRAY(choice, sw->FuncForSCPI(i));
}
