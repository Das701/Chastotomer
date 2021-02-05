#include "defines.h"
#include "Utils/Buffer.h"
#include "Utils/Stack.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#include <iomanip>
#include <cstring>
#include <cctype>
#include <sstream>
#include <cstdlib>
#ifdef WIN32
#include <cstring>
#endif

#ifndef LANG
#define LANG 0
#define LANG_RU true
#endif


// Возвращает false, если выбор невозможен - строка кончилась.
static bool ChooseSymbols(const char **string);
// Возвращает false, если выбор невозможен - строка кончилась.
static bool ChooseSpaces(const char **string);


bool String2Int(char *str, int *value)
{
    int sign = str[0] == '-' ? -1 : 1; //-V2563
    if (str[0] < '0' || str[0] > '9') //-V2563
    {
        str++;
    }
    uint length = std::strlen(str); //-V2513
    if (length == 0)
    {
        return false;
    }

    *value = 0;
    int pow = 1;
    uint i = length;
    do
    {
        --i;
        int val = str[i] & (~(0x30)); //-V2563
        if (val < 0 || val > 9)
        {
            return false;
        }
        *value += val * pow;
        pow *= 10;
    } while (i > 0);

    if (sign == -1)
    {
        *value *= -1;
    }
    return true;
}


int BCD2Int(uint bcd)
{
    uint pow = 1;

    int value = 0;

    for (int i = 0; i < 8; i++)
    {
        value += (bcd & 0x0f) * pow;
        pow *= 10;
        bcd = bcd >> 4;
    }

    return value;
}


#define  SYMBOL(x) (*(*(x)))


static bool ChooseSymbols(const char **string)
{
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a) //-V2563
    {
        return false;
    }

    while (SYMBOL(string) != ' ' && SYMBOL(string) != 0x0d && SYMBOL(string + 1) != 0x0a) //-V2563
    {
        (*string)++;
    }

    return true;
}


static bool ChooseSpaces(const char **string)
{
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a) //-V2563
    {
        return false;
    }

    while (SYMBOL(string) == ' ')
    {
        (*string)++;
    }

    return true;
}

#undef SYMBOL


bool SU::GetWord(const char *string, Word *word, const int numWord)
{
    ChooseSpaces(&string);

    int currentWord = 0;

    while (true)
    {
        if (currentWord == numWord)
        {
            word->address = const_cast<char *>(string); //-V2567
            ChooseSymbols(&string);
            word->numSymbols = static_cast<int8>(string - word->address);

            char *pointer = word->address;
            int numSymbols = word->numSymbols;
            for (int i = 0; i < numSymbols; i++)
            {
                *pointer = static_cast<char>(std::toupper(*pointer));
                pointer++;
            }
            return true;
        }
        if (ChooseSymbols(&string))
        {
            currentWord++;
        }
        else
        {
            return false;
        }
        ChooseSpaces(&string);
    }
}


bool SU::WordEqualZeroString(Word *word, pCHAR string)
{
    pCHAR ch = string;
    char *w = word->address;

    while (*ch != 0)
    {
        if (*ch++ != *w++)
        {
            return false;
        }
    }

    return (ch - string) == word->numSymbols;
}


bool SU::EqualsZeroStrings(char *str1, char *str2)
{
    while ((*str1) == (*str2))
    {
        if ((*str1) == '\0')
        {
            return true;
        }
        str1++;
        str2++;
    }
    return false;
}


bool SU::EqualsStrings(uint8 *str1, const char * const str2, int size)
{
    return EqualsStrings(static_cast<void *>(str1), const_cast<char *>(str2), size); //-V2567
}


bool SU::EqualsStrings(void *_str1, void *_str2, int size)
{
    char *str1 = static_cast<char *>(_str1); //-V2571
    char *str2 = static_cast<char *>(_str2); //-V2571

    for (int i = 0; i < size; i++)
    {
        if (str1[i] != str2[i]) //-V2563
        {
            return false;
        }
    }
    return true;
}


bool SU::EqualsStrings(const char *str1, const char *str2)
{
    uint size = std::strlen(str1); //-V2513

    for (uint i = 0; i < size; i++)
    {
        if (str1[i] != str2[i]) //-V2563
        {
            return false;
        }
    }
    return true;
}


int SU::FirstNotNumeral(char *buffer)
{
    int result = 0;

    while (*buffer++)
    {
        char symbol = *buffer;

        if ((symbol < 0x30 || symbol > 0x39) && symbol != '.')
        {
            break;
        }

        result++;
    }

    return result;
}


float SU::StringToFloat(const char *string)
{
    float result = 0.0F;

    Stack<int8> stack(20);

    while (*string)
    {
        char symbol = *string;
        if (symbol < 0x30 || symbol > 0x39)
        {
            break;
        }
        stack.Push(symbol & 0x0f);
        string++;
    }

    {
        int pow = 1;

        while (stack.Size() > 0)
        {
            result += static_cast<float>(pow) * (float)stack.Pop();
            pow *= 10;
        }
    }

    // Теперь в result целая часть числа

    if (*string == '.')
    {
        string++;

        float pow = 0.1F;

        while (*string)
        {
            char symbol = *string;
            if (symbol < 0x30 || symbol > 0x39)
            {
                break;
            }
            result += pow * (float)(symbol & 0x0f);
            pow /= 10.0F;
            string++;
        }
    }


    return result;
}


char *SU::ToUpper(char *str)
{
    return ToUpper(str, static_cast<int>(std::strlen(str))); //-V2513
}


char *SU::ToUpper(char *_str, int size)
{
    char *str = static_cast<char *>(_str);

    for(int i = 0; i < size; i++)
    {
        str[i] = static_cast<char>(std::toupper(str[i])); //-V2563
    }

    return str;
}


char SU::ToUpper(char symbol)
{
    uint8 s = static_cast<uint8>(symbol);

    if(s == 0x40)        // ё
    {
        return '\xc5';
    }
    else if(s >= 0x60 && s <= 0x7a)
    {
        return static_cast<char>(s - 0x20);
    }
    else if(s >= 0xf0) //-V2516
    {
        return static_cast<char>(s - 0x20);
    }

    return symbol;
}


char SU::ToLower(char symbol)
{
    uint8 s = static_cast<uint8>(symbol);

    if(s >= 0x41 && s <= 0x5a)
    {
        return static_cast<char>(s + 0x20);
    }
    else if(s >= 0xc0 && s < 0xE0) //-V2516
    {
        return static_cast<char>(s + 0x20);
    }

    return symbol;
}


#ifdef VS_KEIL

char *SU::DoubleToString(double)
{
    return nullptr;
}

#else

char *SU::DoubleToString(double value)
{
    static char result[100];

    std::stringstream stream;
    stream << std::fixed << std::setprecision(5) << value;

    strcpy_s(result, 90, stream.str().c_str());

    char *p = result;

    while (*p)
    {
        if (*p == '.')
        {
            *p = ',';
        }
        p++;
    }

    return result;
}

#endif


bool SU::StringToDouble(double *value, const char *str)
{
    static const int SIZE_BUFFER = 100;
    char buffer[SIZE_BUFFER];

    strcpy_s(buffer, SIZE_BUFFER - 1, str);

    char *p = buffer;

    while (*p)
    {
        if (*p == '.')
        {
            *p = ',';
        }
        p++;
    }

    char *end = nullptr;

    *value = std::strtod(buffer, &end);

    return (end != buffer);
}


bool SU::String2Int(const char *buffer, int *value, char **end)
{
    Buffer string(static_cast<int>(std::strlen(buffer)) + 1); //-V2513

    std::strcpy(string.DataChar(), buffer); //-V2513

    *value = std::strtol(string.DataChar(), end, 10);

    if (*end == string.DataChar())
    {
        *end = const_cast<char *>(buffer); //-V2567
    }
    else
    {
        *end = const_cast<char *>(buffer) + (*end - string.DataChar()); //-V2563 //-V2567
    }

    return (*end != const_cast<char *>(buffer)); //-V2567
}


#ifndef WIN32

int strcpy_s(char *dest, uint dest_size, const char *src)
{
    if (std::strlen(src) + 1 < dest_size)
    {
        std::strcpy(dest, src);
        return 0;
    }

    return 1;
}

#endif



static void Swap(char *ch1, char *ch2)
{
    char c = *ch1;
    *ch1 = *ch2;
    *ch2 = c;
}


String SU::Int2String(int n)
{
    char buffer[20];

    char *result = buffer;

    if (n < 0)
    {
        buffer[0] = '-';
        n = -n;
        result = &buffer[1];
    }

    int i = 0;

    do
    {
        result[i++] = n % 10 + '0';
        n -= n % 10;
        n /= 10;
    } while (n > 0);

    result[i] = '\0';

    for (int j = 0; j < i; j++)
    {
        i--;
        Swap(&result[j], &result[i]);
    }

    return String(buffer);
}


void SU::LeaveFewDigits(char *buffer, int size, int digits)
{
    char *pointer = buffer;

    while (*pointer != '\0' && digits != 0 && size > 0)
    {
        size--;

        if ((*pointer & 0xF0) == 0x30)
        {
            digits--;
        }

        pointer++;
    }

    if (size > 0)
    {
        *pointer = '\0';
    }
}


void SU::LeaveFewSignedDigits(char *buffer, int size, int digits)
{
    bool isControl = false;

    char *pointer = buffer;

    while (*pointer != '\0' && digits != 0 && size > 0)
    {
        size--;

        if ((*pointer & 0xF0) == 0x30)
        {
            if (*pointer == 0x30 && !isControl)
            {
                pointer++;
                continue;
            }

            isControl = true;

            digits--;
        }

        pointer++;
    }

    if (*pointer == '.' || *pointer == ',')
    {
        pointer++;
    }

    if (size > 0)
    {
        *pointer = '\0';
    }
}


void SU::ReplaceSymbol(char *string, char oldSymbol, char newSymbol)
{
    while (*string != '\0')
    {
        if (*string == oldSymbol)
        {
            *string = newSymbol;
        }

        string++;
    }
}


bool SU::ExistSymbol(char *string, char symbol)
{
    while (*string != '\0')
    {
        if (*string == symbol)
        {
            return true;
        }

        string++;
    }

    return false;
}


bool SU::IsDigit(char symbol)
{
    return symbol >= '0' && symbol <= '9';
}
