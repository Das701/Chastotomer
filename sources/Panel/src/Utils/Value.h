#pragma once
#include "Utils/String.h"


struct Order
{
    enum E
    {
        Mega,
        Kilo,
        One,
        Milli,
        Micro,
        Nano,
        Count
    };

    // Возвращает степень, соответсвующую порядку
    static int GetPow10(Order::E order);

    // Сохранить order во внутренней памяти
    static void Store(Order::E order);

    // Восстановить значение, ранее сохранённое функцией Store()
    static Order::E Restore();
};


struct int128
{
    static int128 IntPartDouble(double v);
    int128(int64 = 0);
    int128(int = 0);
    operator int64() const;
    int128 operator /(int128 &second);
};


int128 operator-(const int128 &first);
int128 operator/(const int128 &first, const int128 &second);
int128 &operator/=(int128 &first, const int64 &second);
int128 &operator*=(int128 &first, const int64 &second);
int128 &operator+=(int128 &first, const int128 &second);
int128 &operator-=(int128 &first, const int128 &second);


struct ValueATTO //-V690
{
    explicit ValueATTO(int64 v = 0);
    ValueATTO(const ValueATTO &v) : sign(v.sign), atto(v.atto) {};

    void FromDouble(double v);

    void Div(int64 div);
    void Mul(int64 mul);

    void Add(const ValueATTO &value);
    void Sub(const ValueATTO &value);

    int Sign() const;
    void SetSign(int sign);

    // Возращает строку значения
    String ToString() const;

    double ToDouble() const;

    int128 Abs() const;

    int64 Integer() const;

    int128 ToATTO() const;

    // Возвращает количество атто-единиц в одной единице
    int128 OneUnit() const;

private:

    int sign;

                        //                                               милли  микро  нано   пико   фемто  атто
                        //                                                -3     -6     -9     -12    -15    -18
    int128 atto;      // Здесь значение хранится в атто-единицах. 1 == 1000 * 1000 * 1000 * 1000 * 1000 * 1000
};


ValueATTO operator/(const ValueATTO &first, uint second);
ValueATTO operator-(const ValueATTO &first, const ValueATTO &second);
ValueATTO operator-(const ValueATTO &first, int second);
