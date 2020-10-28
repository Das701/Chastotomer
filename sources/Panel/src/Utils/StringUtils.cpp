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
    int sign = str[0] == '-' ? -1 : 1;
    if (str[0] < '0' || str[0] > '9')
    {
        str++;
    }
    uint length = std::strlen(str);
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
        int val = str[i] & (~(0x30));
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


char *SU::Db2String(float value, int numDigits, char bufferOut[20])
{
    bufferOut[0] = 0;
    std::strcat(bufferOut, Float(value).ToString(false, numDigits).c_str());
    std::strcat(bufferOut, "дБ");
    return bufferOut;
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
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a)
    {
        return false;
    }

    while (SYMBOL(string) != ' ' && SYMBOL(string) != 0x0d && SYMBOL(string + 1) != 0x0a)
    {
        (*string)++;
    }

    return true;
}


static bool ChooseSpaces(const char **string)
{
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a)
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
            word->address = const_cast<char *>(string);
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


bool SU::WordEqualZeroString(Word *word, char* string)
{
    char *ch = string;
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
    return EqualsStrings(static_cast<void *>(str1), const_cast<char *>(str2), size);
}


bool SU::EqualsStrings(void *_str1, void *_str2, int size)
{
    char *str1 = static_cast<char *>(_str1);
    char *str2 = static_cast<char *>(_str2);

    for (int i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}


bool SU::EqualsStrings(const char *str1, const char *str2)
{
    uint size = std::strlen(str1);

    for (uint i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
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
            result += static_cast<float>(pow) * stack.Pop();
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
            result += pow * (symbol & 0x0f);
            pow /= 10.0F;
            string++;
        }
    }


    return result;
}


char *SU::ToUpper(char *str)
{
    return ToUpper(str, static_cast<int>(std::strlen(str)));
}


char *SU::ToUpper(char *_str, int size)
{
    char *str = static_cast<char *>(_str);

    for(int i = 0; i < size; i++)
    {
        str[i] = static_cast<char>(std::toupper(str[i]));
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
    else if(s >= 0xf0)
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
    else if(s >= 0xc0 && s < 0xE0)
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
    Buffer string(static_cast<int>(std::strlen(buffer)) + 1);

    std::strcpy(string.DataChar(), buffer);

    *value = std::strtol(string.DataChar(), end, 10);

    if (*end == string.DataChar())
    {
        *end = const_cast<char *>(buffer);
    }
    else
    {
        *end = const_cast<char *>(buffer) + (*end - string.DataChar());
    }

    return (*end != const_cast<char *>(buffer));
}


#ifndef GUI

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

