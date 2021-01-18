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

    atto = int128::IntPartDouble(std::fabs(v) * 1e18);
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
    return (int128)1000 * 1000 * 1000 * 1000 * 1000 * 1000;
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


int128 int128::IntPartDouble(double v)
{
    return -1;
}
