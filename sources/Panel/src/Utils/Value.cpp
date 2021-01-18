#include "defines.h"
#include "Utils/Math.h"
#include "Utils/Stack.h"
#include "Utils/Value.h"
#include <cmath>
#include <cstring>


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


//static bool GetSign(int &sign, char *begin, char **end) //-V2506
//{
//    if (*begin == '-')
//    {
//        *end = begin + 1; //-V2563
//        sign = -1;
//        return true;
//    }
//    else if (*begin == '+') //-V2516
//    {
//        *end = begin + 1; //-V2563
//        sign = 1;
//        return true;
//    }
//
//    if (*begin >= '0' && *begin <= '9')
//    {
//        *end = begin;
//        sign = 1;
//        return true;
//    }
//
//    *end = begin;
//    return false;
//}


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
//static bool GetIntPart(ValueATTO &, char *, char **, int)
//{
//    return true;
//}
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
//static bool GetPower(int &, char *, char **)
//{
//    return true;
//}
#endif


//static void ProcessSign(const char *const buffer, int *pos, int *sign)
//{
//    if (buffer[0] == '+') //-V2563
//    {
//        *sign = 1;
//        *pos = *pos + 1;
//    }
//    else if (buffer[0] == '-') //-V2516 //-V2563
//    {
//        *sign = -1;
//        *pos = *pos + 1;
//    }
//}


//static int FindIntegerPart(const char *const buffer, int start)
//{
//    int pos = start;
//
//    while (buffer[pos] >= '0' && buffer[pos] <= '9') //-V2563
//    {
//        pos++;
//    }
//
//    return pos;
//}


//static uint AssembleInteger(const char *const buffer, int start, int end)
//{
//    char stack[20];
//
//    int posStack = 0;
//
//    for (int i = start; i < end; i++)
//    {
//        stack[posStack++] = buffer[i]; //-V2563
//    }
//
//    uint result = 0;
//
//    uint pow = 1;
//
//    while (posStack > 0)
//    {
//        char value = stack[(posStack--) - 1];
//
//        result += (value & 0x0F) * pow;
//
//        pow *= 10;
//    }
//
//    return result;
//}


//static uint AssembleTriple(const char *const buffer, int start, int *end)
//{
//    char stack[3];
//    int posStack = 0;
//
//    uint result = 0;
//
//    int i = start;
//
//    while ((buffer[i] >= '0' && buffer[i] <= '9') && //-V2563
//        (posStack < 3))
//    {
//        stack[posStack] = buffer[i]; //-V2563
//        posStack++;
//        i++;
//    }
//
//    while (posStack < 3)                    // Добиваем до 3 символов
//    {
//        stack[posStack++] = '0';
//    }
//
//    *end = i;
//
//    uint pow = 1;
//
//    while (posStack > 0)
//    {
//        char value = stack[(posStack--) - 1];
//
//        result += (value & 0x0F) * pow;
//
//        pow *= 10;
//    }
//
//    return result;
//}


int64 ValueATTO::Integer() const
{
    int128 val = Abs();

    return (int64)(val / OneUnit()) * Sign();
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


void ValueATTO::FromDouble(double v)
{
    sign = (v < 0.0) ? -1 : 1;

    v = std::fabs(v);

    atto = std::fabs(v) * 1e18;
}


void ValueATTO::Div(int64 div)
{
    sign *= (div < 0) ? -1 : 1;

    atto /= div;
}


void ValueATTO::Mul(int64 mul)
{
    sign *= (mul < 0) ? -1 : 1;

    atto *= mul;
}


void ValueATTO::Add(const ValueATTO &add)
{
    if (Sign() > 0)
    {
        if (add.Sign() > 0)             // left > 0 ; right > 0
        {
            atto += add.atto;
        }
        else                            // left > 0 ; right < 0
        {
            atto -= add.atto;

            if (atto < 0)
            {
                atto = -atto;
                SetSign(-1);
            }
        }
    }
    else
    {
        if (add.Sign() < 0)             // left < 0 ; right < 0
        {
            atto += add.atto;
        }
        else                            // left < 0; right > 0
        {
            int128 temp = add.atto;
            temp -= atto;
            atto = temp;

            if (atto < 0)
            {
                atto = -atto;
                SetSign(1);
            }
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
    return sign;
}


void ValueATTO::SetSign(int s)
{
    sign = s;
}


String ValueATTO::ToString() const
{
    char buffer[50];

    char symbol[2] = { 0, 0 };

    buffer[0] = 0;

    int64 intPart = Integer();

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
        
        int64 integer = val.Integer();
        
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


int128 ValueATTO::Abs() const
{
    return atto;
}


ValueATTO::ValueATTO(int64 v) : sign(1), atto(0)
{
    SetSign(v < 0 ? -1 : 1);

    if (Sign() < 0)
    {
        v = -v;
        atto = v * OneUnit();
    }
}


int128 ValueATTO::ToATTO() const
{
    return atto * Sign();
}


int128 ValueATTO::OneUnit() const
{
    return 1000 * 1000 * 1000 * 1000 * 1000 * 1000;
}


int128::int128(double v)
{

}


int128::int128(int v)
{

}


int128::int128(int64 v)
{

}


int128::operator int64() const
{
    return -1;
}


int128 operator-(const int128 &first)
{
    return -1;
}


int128 operator/(const int128 &first, const int128 &second)
{
    return (int128)-1;
}


int128 &operator/=(int128 &first, const int64 &second)
{
    return first;
}


int128 &operator*=(int128 &first, const int64 &second)
{
    return first;
}


int128 &operator+=(int128 &first, const int128 &second)
{
    return first;
}


int128 &operator-=(int128 &first, const int128 &second)
{
    return first;
}
