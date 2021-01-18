#include "defines.h"
#include "Utils/Math.h"
#include "Utils/Stack.h"
#include "Utils/Value.h"
#include <cmath>
#include <cstring>


struct MathDouble
{
    static int GetPositionFirstDigit(const ValueATTO &value, Order::E order);

    // Возвращает символ в позиции position. Знак не учитывается. Точка находится соответственно order. One - после единиц, Kilo - после тысяч и так далее.
    // Order::Count - значенине по умолчанию - запятая в позиции относительно размерности числового значения
    static char GetChar(const ValueATTO &value, int postition, Order::E order);

    // Возвращает цифру в позиции position. Точка находится соответственно order. One - после единиц, Kilo - после тысяч и так далее.
    // Order::Count - значенине по умолчанию - запятая в позиции относительно размерности числового значения
    static int GetDigit(const ValueATTO &value, int position, Order::E order = Order::Count);
};



int MathDouble::GetPositionFirstDigit(const ValueATTO &val, Order::E order) //-V2506
{
    ValueATTO value = val;
    value.SetSign(1);

    int result = 0;

    if (value.Integer() > 0)
    {
        int whole = value.Integer();

        while (whole > 9)
        {
            whole /= 10;
            result++;
        }
    }
    else
    {
        uint64 fract = value.FractPico();

        if (fract == 0)
        {
            return 0;
        }

        do
        {
            result--;
            fract *= 10;
        } while (fract < (100 * 1000 * 1000));
    }

    return result - Order::GetPow10(order == Order::Count ? value.GetOrder() : order);
}


char MathDouble::GetChar(const ValueATTO &value, int position, Order::E order)
{
    int digit = GetDigit(value, position, order);

    return (digit == -1) ? '\0' : static_cast<char>(GetDigit(value, position, order) | 0x30);
}


int MathDouble::GetDigit(const ValueATTO &val, int position, Order::E order) //-V2506
{
    ValueATTO value = val;
    value.SetSign(1);

    order = (order == Order::Count) ? value.GetOrder() : order;

    position += Order::GetPow10(order);

    if (position < 0)
    {
        int divider = 100 * 1000 * 1000;       // На это число будем делить количество наносекунд

        uint64 fract = value.FractPico();

        while (position < -1)
        {
            if (divider == 0)
            {
                return -1;
            }

            fract %= divider;
            divider /= 10;
            position++;
        }

        return (int)((divider == 0) ? -1 : (fract / divider));
    }
    else
    {
        int whole = value.Integer();

        while (position > 0)
        {
            whole /= 10;
            position--;
        }

        return (whole % 10);
    }
}


// Находит знак, если первый элемент buffer - знак. В pos записывается позиция элемента за знаком в этом случае
static void ProcessSign(const char *const buffer, int *pos, int *sign);

// Находит позицю за последним элементом целой части числа
static int FindIntegerPart(const char *const buffer, int start);

// Собрать целое число из строки символов
static uint AssembleInteger(const char *const buffer, int start, int end);

// Собрать число из трех или менее символов. В end возвращается позиция следующего символа
static uint AssembleTriple(const char *const buffer, int start, int *end);

// Место для временного сохранения текущего порядка
static Order::E stored = Order::Count;

// Читает знак числа из buffer. Возвращает указатель на первый элемент после знака
static bool GetSign(int &sign, char *begin, char **end);

// Возвращает значение до степени (символа e)
static bool GetIntPart(ValueATTO &value, char *begin, char **end, int numDigitsAfterComma);

// Возвращает степень (то, что после символа e)
static bool GetPower(int &power, char *begin, char **end);


static bool GetSign(int &sign, char *begin, char **end) //-V2506
{
    if (*begin == '-')
    {
        *end = begin + 1; //-V2563
        sign = -1;
        return true;
    }
    else if (*begin == '+') //-V2516
    {
        *end = begin + 1; //-V2563
        sign = 1;
        return true;
    }

    if (*begin >= '0' && *begin <= '9')
    {
        *end = begin;
        sign = 1;
        return true;
    }

    *end = begin;
    return false;
}


#ifdef PANEL
static bool GetIntPart(ValueNANO &value, char *begin, char **end, int numDigitsAfterComma)
{
    *end = begin;

    if (*begin < '0' || *begin > '9')
    {
        return false;
    }

    String buffer;

    bool comma = false;         // true будет означать, что точка уже была

    while ((**end >= '0' && **end <= '9') || **end == '.' || **end == ',')
    {
        if (**end == '.' || **end == ',')
        {
            comma = true;
        }
        else if (comma)
        {
            numDigitsAfterComma--;
        }

        if (numDigitsAfterComma >= 0)
        {
            buffer.Append(**end);
        }

        *end = *end + 1;
    }

    value = ValueNANO(buffer.c_str(), Order::One);

    return true;
}
#else
static bool GetIntPart(ValueATTO &, char *, char **, int)
{
    return true;
}
#endif


#ifdef PANEL
static bool GetPower(int &pow, char *begin, char **end)
{
    if (*begin != 'e' && *begin != 'E')
    {
        *end = begin;
        pow = 0;
        return true;
    }

    begin++;

    if ((*begin < '0' || *begin > '9') && (*begin != '-' && *begin != '+'))
    {
        *end = begin;
        pow = 0;
        return true;
    }

    String buffer;

    while ((*begin >= '0' && *begin <= '9') || *begin == '-' || *begin == '+')
    {
        buffer.Append(*begin);
        begin++;
    }

    *end = begin;

    pow = ValueNANO(buffer.c_str(), Order::One).Integer();

    return true;
}
#else
static bool GetPower(int &, char *, char **)
{
    return true;
}
#endif


static void ProcessSign(const char *const buffer, int *pos, int *sign)
{
    if (buffer[0] == '+') //-V2563
    {
        *sign = 1;
        *pos = *pos + 1;
    }
    else if (buffer[0] == '-') //-V2516 //-V2563
    {
        *sign = -1;
        *pos = *pos + 1;
    }
}


static int FindIntegerPart(const char *const buffer, int start)
{
    int pos = start;

    while (buffer[pos] >= '0' && buffer[pos] <= '9') //-V2563
    {
        pos++;
    }

    return pos;
}


static uint AssembleInteger(const char *const buffer, int start, int end)
{
    char stack[20];

    int posStack = 0;

    for (int i = start; i < end; i++)
    {
        stack[posStack++] = buffer[i]; //-V2563
    }

    uint result = 0;

    uint pow = 1;

    while (posStack > 0)
    {
        char value = stack[(posStack--) - 1];

        result += (value & 0x0F) * pow;

        pow *= 10;
    }

    return result;
}


static uint AssembleTriple(const char *const buffer, int start, int *end)
{
    char stack[3];
    int posStack = 0;

    uint result = 0;

    int i = start;

    while ((buffer[i] >= '0' && buffer[i] <= '9') && //-V2563
        (posStack < 3))
    {
        stack[posStack] = buffer[i]; //-V2563
        posStack++;
        i++;
    }

    while (posStack < 3)                    // Добиваем до 3 символов
    {
        stack[posStack++] = '0';
    }

    *end = i;

    uint pow = 1;

    while (posStack > 0)
    {
        char value = stack[(posStack--) - 1];

        result += (value & 0x0F) * pow;

        pow *= 10;
    }

    return result;
}


int ValueATTO::Integer() const
{
    return (int)(Abs() / 1000 / 1000 / 1000 / 1000) * Sign(); //-V2533
}


uint64 ValueATTO::FractPico() const
{
    ValueATTO val(*this);

    val.SetSign(1);

    int whole = val.Integer();

    return (val.value - whole * 1000 * 1000 * 1000 * 1000);
}


void Order::Store(Order::E order)
{
    stored = order;
}


Order::E Order::Restore()
{
    return stored;
}


int Order::GetPow10(Order::E order)
{
    static const int pows[Count] =
    {
        6,  // Mega
        3,  // Kilo
        0,  // One
        -3, // Milli
        -6, // Micro
        -9  // Nano
    };

    return pows[order];
}


static void AddChar(char *buffer, const ValueATTO &value, int pos, Order::E order)
{
    char digit[2] = { 0, 0 };
    digit[0] = MathDouble::GetChar(value, pos, order);
    std::strcat(buffer, digit); //-V2513
}


ValueATTO::ValueATTO(int v)
{
    FromINT(v);
}


ValueATTO::ValueATTO(const ValueATTO &v) : value(v.value)
{
}


void ValueATTO::FromINT(int v)
{
    FromUNITS(v < 0 ? -v : v, 0, 0, 0, 0, v < 0 ? -1 : 1);
}


void ValueATTO::FromDouble(double v)
{

}


uint64 ValueATTO::ToUINT64() const
{
    return value;
}


Order::E ValueATTO::GetOrder() const
{
    return Order::One;
}


void ValueATTO::FromUNITS(int units, uint mUnits, uint uUnits, uint nUnits, uint pUnits, int sign)
{
    value = (uint64)units; //-V2533

    value = value * 1000 * 1000 * 1000 * 1000;

    value += (uint64)pUnits + (uint64)nUnits * 1000 + (uint64)uUnits * 1000 * 1000 + (uint64)mUnits * 1000 * 1000 * 1000; //-V2533

    if (sign < 0)
    {
        SetSign(sign);
    }
}


void ValueATTO::Div(uint div)
{
    int sign = Sign();

    SetSign(1);

    value /= div;

    SetSign(sign);
}


void ValueATTO::Mul(uint mul)
{
    int sign = Sign();

    SetSign(1);

    value *= mul;

    SetSign(sign);
}


void ValueATTO::Add(ValueATTO &add)
{
    int sign = Sign();
    int signAdd = add.Sign();

    SetSign(1);
    add.SetSign(1);

    if (sign > 0 && signAdd > 0)
    {
        value += add.value;
    }
    else if (sign < 0 && signAdd < 0)
    {
        value += add.value;
        SetSign(-1);
    }
    else if (sign > 0 && signAdd < 0)
    {
        if (value >= add.value)
        {
            value -= add.value;
        }
        else
        {
            value = add.value - value;
            SetSign(-1);
        }
    }
    else
    {
        if (add.value >= value)
        {
            value = add.value - value;
        }
        else
        {
            value -= add.value;
            SetSign(-1);
        }
    }
}


void ValueATTO::Sub(const ValueATTO &val)
{
    ValueATTO sub(val);

    sub.SetSign(-val.Sign());

    Add(sub);
}


int ValueATTO::Sign() const
{
    //                fedcba9876543210
    return (value & 0x8000000000000000U) ? -1 : 1;
}


void ValueATTO::SetSign(int sign)
{
    if (sign >= 0)
    {
        //         fedcba9876543210
        value &= 0x7FFFFFFFFFFFFFFFU;   // Сбрасываем старший бит - признак положительного числа
    }
    else
    {
        //         fedcba9876543210
        value |= 0x8000000000000000U;   // Устанавливаем старший бит - признак отрицательного числа
    }
}


String ValueATTO::ToString() const
{
    char buffer[50];

    char symbol[2] = { 0, 0 };

    buffer[0] = 0;

    int intPart = Integer();

    Stack<char> stack(100);

    while (intPart != 0)
    {
        stack.Push(intPart % 10);
        intPart /= 10;
    }
    
    if(stack.Empty())
    {
        symbol[0] = '0';
        std::strcat(buffer, symbol); //-V2513
    }

    while (!stack.Empty())                          // Переводим в строку целую часть
    {
        symbol[0] = stack.Pop() | 0x30;

        std::strcat(buffer, symbol); //-V2513
    }

    symbol[0] = '.';

    std::strcat(buffer, symbol); //-V2513

    ValueATTO val(*this);

    val.Sub(ValueATTO(Integer()));                  // Теперь в val осталась только дробная часть

    int count = 0;

    while (count < 4)
    {
        val.Mul(10);
        
        int integer = val.Integer();
        
        symbol[0] = (char)(integer | 0x30); //-V2533

        std::strcat(buffer, symbol); //-V2513

        count++;

        val.Sub(ValueATTO(integer));
    }

    return String(buffer);
}


double ValueATTO::ToDouble() const
{
    return (double)Abs() / 1E12 * (double)Sign(); //-V2533
}


uint64 ValueATTO::Abs() const
{   //                fedcba9876543210
    return (value & 0x7fffffffffffffff);
}
