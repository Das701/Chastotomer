#include "defines.h"
#include "Utils/Math.h"
#include "Utils/Stack.h"
#include "Utils/Value.h"
#include <cmath>
#include <cstring>


struct MathDouble
{
    static int GetPositionFirstDigit(const ValueNANO &value, Order::E order);

    // Возвращает символ в позиции position. Знак не учитывается. Точка находится соответственно order. One - после единиц, Kilo - после тысяч и так далее.
    // Order::Count - значенине по умолчанию - запятая в позиции относительно размерности числового значения
    static char GetChar(const ValueNANO &value, int postition, Order::E order);

    // Возвращает цифру в позиции position. Точка находится соответственно order. One - после единиц, Kilo - после тысяч и так далее.
    // Order::Count - значенине по умолчанию - запятая в позиции относительно размерности числового значения
    static int GetDigit(const ValueNANO &value, int position, Order::E order = Order::Count);
};



int MathDouble::GetPositionFirstDigit(const ValueNANO &val, Order::E order)
{
    ValueNANO value = val;
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
        int fract = value.FractNano();

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


char MathDouble::GetChar(const ValueNANO &value, int position, Order::E order)
{
    int digit = GetDigit(value, position, order);

    return (digit == -1) ? '\0' : static_cast<char>(GetDigit(value, position, order) | 0x30);
}


int MathDouble::GetDigit(const ValueNANO &val, int position, Order::E order)
{
    ValueNANO value = val;
    value.SetSign(1);

    order = (order == Order::Count) ? value.GetOrder() : order;

    position += Order::GetPow10(order);

    if (position < 0)
    {
        int divider = 100 * 1000 * 1000;       // На это число будем делить количество наносекунд

        int fract = value.FractNano();

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

        return (divider == 0) ? -1 : (fract / divider);
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
static bool GetIntPart(ValueNANO &value, char *begin, char **end, int numDigitsAfterComma);

// Возвращает степень (то, что после символа e)
static bool GetPower(int &power, char *begin, char **end);


ValueNANO::ValueNANO(const char *const buffer, Order::E order) //-V730
{
    FromString(buffer, Order::GetPow10(order));
}


ValueNANO::ValueNANO(double v)
{
    FromDouble(v);
}


ValueNANO::ValueNANO(int v)
{
    FromINT(v);
}


bool ValueNANO::FromString(const char *buffer, char **end, int numDigitsAfterComma)
{
    char *begin = const_cast<char *>(buffer);

    int sign = 0;

    if (!GetSign(sign, begin, end))
    {
        return false;
    }

    begin = *end;

    if (!GetIntPart(*this, begin, end, numDigitsAfterComma))
    {
        return false;
    }

    begin = *end;

    int pow = 0;

    if (!GetPower(pow, begin, end))
    {
        return false;
    }

    MulPow10(pow);

    SetSign(sign);

    return true;
}


static bool GetSign(int &sign, char *begin, char **end)
{
    if (*begin == '-')
    {
        *end = begin + 1;
        sign = -1;
        return true;
    }
    else if (*begin == '+')
    {
        *end = begin + 1;
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
static bool GetIntPart(ValueNANO &, char *, char **, int)
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


void ValueNANO::FromString(const char *const buffer, int pow10)
{
    int pos = 0;                                    // Текущая обрабатываемая позиция в buffer
    int sign = 1;                                   // Отрицательное значение означает отрицательный знак

    ProcessSign(buffer, &pos, &sign);               // Обрабатываем информацию о знаке

    int units = 0;
    uint mUnits = 0;
    uint uUnits = 0;
    uint nUnits = 0;

    int end = FindIntegerPart(buffer, pos);         // Находим окончание целой части

    units = static_cast<int>(AssembleInteger(buffer, pos, end));    // Находим целую часть

    if (buffer[end] == '.' || buffer[end] == ',')
    {
        mUnits = AssembleTriple(buffer, end + 1, &end);
        uUnits = AssembleTriple(buffer, end, &end);
        nUnits = AssembleTriple(buffer, end, &end);
    }

    FromUnits(units, mUnits, uUnits, nUnits, sign);

    if (pow10 > 0)
    {
        uint pow = (uint)Math::Pow10(pow10);
        Mul(pow);
    }
    else if (pow10 < 0)
    {
        uint pow = (uint)Math::Pow10(-pow10);
        Div(pow);
    }
}


void ValueNANO::FromUnits(int units, uint mUnits, uint uUnits, uint nUnits, int sign)
{
    value = static_cast<uint>(units);
    value *= 1000 * 1000 * 1000;

    value += static_cast<uint>(nUnits + uUnits * 1000 + mUnits * 1000 * 1000);

    if (sign < 0)
    {
        SetSign(sign);
    }
}


static void ProcessSign(const char *const buffer, int *pos, int *sign)
{
    if (buffer[0] == '+')
    {
        *sign = 1;
        *pos = *pos + 1;
    }
    else if (buffer[0] == '-')
    {
        *sign = -1;
        *pos = *pos + 1;
    }
}


static int FindIntegerPart(const char *const buffer, int start)
{
    int pos = start;

    while (buffer[pos] >= '0' && buffer[pos] <= '9')
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
        stack[posStack++] = buffer[i];
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

    while ((buffer[i] >= '0' && buffer[i] <= '9') &&
        (posStack < 3))
    {
        stack[posStack] = buffer[i];
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


void ValueNANO::FromDouble(double v)
{
    int sign = (v < 0.0) ? -1 : 1;

    value = static_cast<uint64>(std::fabs(v) * 1.E9);

    if (sign < 0)
    {
        SetSign(-1);
    }
}


void ValueNANO::FromINT(int v)
{
    FromUnits(v < 0 ? -v : v, 0, 0, 0, v < 0 ? - 1 : 1);
}


double ValueNANO::ToDouble() const
{
    return static_cast<double>(Abs()) / 1E9 * static_cast<double>(Sign());
}


float ValueNANO::ToFloat() const
{
    return (float)ToDouble();
}


int ValueNANO::Sign() const
{
    //                fedcba9876543210
    return (value & 0x8000000000000000U) ? -1 : 1;
}


uint64 ValueNANO::Abs() const
{   //                fedcba9876543210
    return (value & 0x7fffffffffffffff);
}


void ValueNANO::Div(uint div)
{
    int sign = Sign();

    SetSign(1);

    value /= div;

    SetSign(sign);
}


void ValueNANO::Mul(uint mul)
{
    int sign = Sign();

    SetSign(1);

    value *= mul;

    SetSign(sign);
}


void ValueNANO::SetSign(int sign)
{
    if (sign >= 0)
    {
        //         fedcba9876543210
        value &= 0x7FFFFFFFFFFFFFFFU;       // \todo как это может работать?
    }
    else
    {
        //         fedcba9876543210
        value |= 0x8000000000000000U;           // Устанавливаем признак отрицательного числа
    }
}


int ValueNANO::Integer() const
{
    uint64 val = Abs();

    return static_cast<int>(val / (1000 * 1000 * 1000)) * Sign();
}


int ValuePICO::Integer() const
{
    return (int)(Abs() / 1000 / 1000 / 1000 / 1000) * Sign();
}


int ValueNANO::FractNano() const
{
    ValueNANO val = *this;
    val.SetSign(1);

    int whole = val.Integer();

    return static_cast<int>(val.value - static_cast<int>(whole * 1000 * 1000 * 1000));
}


uint64 ValuePICO::FractPico() const
{
    ValuePICO val(*this);

    val.SetSign(1);

    int whole = val.Integer();

    return (val.value - whole * 1000 * 1000 * 1000 * 1000);
}


void ValueNANO::Add(ValueNANO add)
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


void ValueNANO::Sub(ValueNANO val)
{
    val.SetSign(-val.Sign());

    Add(val);
}


void ValueNANO::MulPow10(int pow)
{
    while (pow > 0)
    {
        Mul(10);
        pow--;
    }

    while (pow < 0)
    {
        Div(10);
        pow++;
    }
}


bool ValueNANO::operator<(const ValueNANO &rhs)
{
    return ToDouble() < rhs.ToDouble();
}


bool ValueNANO::operator<=(const ValueNANO &rhs)
{
    return ToDouble() <= rhs.ToDouble();
}


bool ValueNANO::operator>(const ValueNANO &rhs)
{
    return ToDouble() > rhs.ToDouble();
}


bool ValueNANO::operator>=(const ValueNANO &rhs)
{
    return ToDouble() >= rhs.ToDouble();
}


bool ValueNANO::operator==(const ValueNANO &rhs)
{
    return (value == rhs.value);
}


bool ValueNANO::operator!=(const ValueNANO &rhs)
{
    return (value != rhs.value);
}


Order::E ValueNANO::GetOrder() const
{
    ValueNANO temp = *this;
    temp.SetSign(1);

    int integer = temp.Integer();

    if (integer >= 1000 * 1000) { return Order::Mega; }
    else if (integer >= 1000) { return Order::Kilo; }
    else if (integer > 0) { return Order::One; }

    int fract = temp.FractNano();

    if (fract >= 1000 * 1000) { return Order::Milli; }
    else if (fract >= 1000) { return Order::Micro; }
    else if (fract > 0) { return Order::Nano; }

    return Order::One;
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


static void AddChar(char *buffer, const ValueNANO &value, int pos, Order::E order)
{
    char digit[2] = { 0, 0 };
    digit[0] = MathDouble::GetChar(value, pos, order);
    std::strcat(buffer, digit);
}


pString ValueNANO::ToString(bool sign, Order::E order) const
{
    static char buffer[50];
    
    std::memset(buffer, '0', 50);

    buffer[0] = '\0';

    if (sign)
    {
        buffer[0] = (Sign() > 0) ? '+' : '-';
        buffer[1] = '\0';
    }

    order = (order == Order::Count) ? GetOrder() : order;

    int first = MathDouble::GetPositionFirstDigit(*this, order);

    for (int i = first; i >= 0; i--)
    {
        AddChar(buffer, *this, i, order);
    }

    std::strcat(buffer, ".");

    for (int i = -1; i >= -9; i--)
    {
        AddChar(buffer, *this, i, order);
    }

    return buffer;
}


ValuePICO::ValuePICO(int v)
{
    FromINT(v);
}


ValuePICO::ValuePICO(const ValuePICO &v) : value(v.value)
{
}


void ValuePICO::FromINT(int v)
{
    FromUNITS(v < 0 ? -v : v, 0, 0, 0, 0, v < 0 ? -1 : 1);
}


void ValuePICO::FromUNITS(int units, uint mUnits, uint uUnits, uint nUnits, uint pUnits, int sign)
{
    value = (uint64)units;

    value = value * 1000 * 1000 * 1000 * 1000;

    value += (uint64)pUnits + (uint64)nUnits * 1000 + (uint64)uUnits * 1000 * 1000 + (uint64)mUnits * 1000 * 1000 * 1000;

    if (sign < 0)
    {
        SetSign(sign);
    }
}


void ValuePICO::Div(uint div)
{
    int sign = Sign();

    SetSign(1);

    value /= div;

    SetSign(sign);
}


void ValuePICO::Mul(uint mul)
{
    int sign = Sign();

    SetSign(1);

    value *= mul;

    SetSign(sign);
}


void ValuePICO::Add(ValuePICO &add)
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


void ValuePICO::Sub(const ValuePICO &val)
{
    ValuePICO sub(val);

    sub.SetSign(-val.Sign());

    Add(sub);
}


int ValuePICO::Sign() const
{
    //                fedcba9876543210
    return (value & 0x8000000000000000U) ? -1 : 1;
}


void ValuePICO::SetSign(int sign)
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


pString ValuePICO::ToString() const
{
    static char buffer[50];

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
        std::strcat(buffer, symbol);
    }

    while (!stack.Empty())                          // Переводим в строку целую часть
    {
        symbol[0] = stack.Pop() | 0x30;

        std::strcat(buffer, symbol);
    }

    symbol[0] = '.';

    std::strcat(buffer, symbol);

    ValuePICO val(*this);

    val.Sub(ValuePICO(Integer()));                  // Теперь в val осталась только дробная часть

    int count = 0;

    while (count < 4)
    {
        val.Mul(10);
        
        int integer = val.Integer();
        
        symbol[0] = (char)(integer | 0x30);

        std::strcat(buffer, symbol);

        count++;

        val.Sub(ValuePICO(integer));
    }

    return buffer;
}


double ValuePICO::ToDouble() const
{
    return (double)Abs() / 1E12 * (double)Sign();
}


uint64 ValuePICO::Abs() const
{   //                fedcba9876543210
    return (value & 0x7fffffffffffffff);
}
