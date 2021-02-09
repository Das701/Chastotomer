#include "defines.h"
#include "Hardware/VCP.h"
#include "Menu/Pages/Channels/Channels.h"
#include "SCPI/SCPI.h"
#include "Utils/Buffer.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include <cstring>


// Рекурсивная функция обработки массива структур StructSCPI.
// В случае успешного выполнения возвращает адрес символа, расположенного за последним обработанным символом.
// В случае неуспешного завершения - возвращает nullptr. Код ошибки находится в *error
static pchar Process(pchar buffer, const StructSCPI structs[]); //-V2504

// Обработка узла дерева node
static pchar ProcessNode(pchar begin, const StructSCPI *node);

// Обработка листа node
static pchar ProcessLeaf(pchar begin, const StructSCPI *node);

// Возвращает true, если символ является началом комнады - разделителем или '*'
static bool IsBeginCommand(const char &symbol);

// Удаляет неправильные символы из начала строки
static void RemoveBadSymbolsFromBegin();

// Удалить последовательность разделителей из начала строки до последнего имеющегося
static bool RemoveSeparatorsSequenceFromBegin();

// Удалить все символы до первого разделителя
static bool RemoveSymbolsBeforeSeparator();

static String data;

static String badSymbols;

void SCPI::AppendNewData(pchar buffer, int size)
{
    data.Append(buffer, size);

    SU::ToUpper(data.c_str());

    RemoveBadSymbolsFromBegin();

    if (data.Size() == 0)
    {
        SendBadSymbols();
    }
}


void SCPI::Update()
{
    RemoveBadSymbolsFromBegin();

    if(data.Size() == 0)
    {
        SendBadSymbols();
        return;
    }

    pchar end = Process(data.c_str(), head);

    if(end)
    {
        data.RemoveFromBegin(static_cast<int>(end - data.c_str()));
    }
}


static pchar Process(pchar buffer, const StructSCPI strct[]) //-V2504
{
    while (!strct->IsEmpty())
    {
        pchar end = SCPI::BeginWith(buffer, strct->key);

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

    badSymbols.Append(*buffer);         // Перебрали все ключи в strct и не нашли ни одного соответствия. Поэтому помещаем начальный разделитель в бракованные символыа

    return buffer + 1;
}


pchar SCPI::BeginWith(pchar buffer, pchar word)
{
    while (*word)
    {
        if (*buffer == '\0')
        {
            return nullptr;
        }

        if (*word == *buffer)
        {
            ++word;
            ++buffer;
        }
        else
        {
            break;
        }
    }

    return (*word == '\0') ? buffer : nullptr;
}


static pchar ProcessNode(pchar begin, const StructSCPI *node)
{
    return Process(begin, node->strct);
}


static pchar ProcessLeaf(pchar begin, const StructSCPI *node)
{
    if (*begin == '\0')                     // Подстраховка от того, что символ окончания команды не принят
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


bool SCPI::IsLineEnding(pchar *buffer)
{
    bool result = (**buffer == 0x0D);

    if (result)
    {
        *(*buffer)++; //-V532
    }

    return result;
}


void SCPI::SendBadSymbols()
{
    if (badSymbols.Size())
    {
        String message("!!! ERROR !!! Invalid sequency : %s", badSymbols.c_str());
        SCPI::SendAnswer(message.c_str());
        badSymbols.Free();
    }
}


static void RemoveBadSymbolsFromBegin()
{
    while (RemoveSymbolsBeforeSeparator() || RemoveSeparatorsSequenceFromBegin())  { }
}


static bool RemoveSymbolsBeforeSeparator()
{
    bool result = false;

    while ((data.Size() != 0) && !IsBeginCommand(data[0]))
    {
        badSymbols.Append(data[0]);
        data.RemoveFromBegin(1);
        result = true;
    }

    return result;
}


static bool RemoveSeparatorsSequenceFromBegin()
{
    bool result = false;

    while (data.Size() > 1 && IsBeginCommand(data[0]) && IsBeginCommand(data[1]))
    {
        badSymbols.Append(data[0]);
        data.RemoveFromBegin(1);
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

        if (static_cast<uint8>(symbol) == 0xa8)     // Тупо пропускаем значок фазы
        {
            continue;
        }

        if (symbol == '\x10')     { symbol = ' '; }
        else if (symbol == 'м')
        {
            if (str[i + 1] == 'к') { message.Append('u'); i++; }
            else                   { message.Append('m');      }
            continue;
        }
        else if (symbol == 'н')    { symbol = 'n'; }
        else if (symbol == 'с')    { symbol = 's'; }
        else if (symbol == 'В')    { symbol = 'V'; }
        else if (symbol == 'А')    { symbol = 'A'; }
        else if (symbol == 'М')    { symbol = 'M'; }
        else if (symbol == 'к')    { symbol = 'k'; }
        else if ((symbol == 'Г') && (str[i + 1] == 'ц')) //-V2516
        {
            message.Append('H');  message.Append('z');  i++; continue;
        }

        message.Append(symbol);
    }

    SendAnswer(message.c_str());
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


void SCPI::AnswerInput(const pchar choice[], uint8 value)
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
