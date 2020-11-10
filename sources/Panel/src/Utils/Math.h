#pragma once


#define LIMITATION(var, min, max)           if(var < (min)) { (var) = (min); } else if(var > (max)) { var = (max); };


namespace Math
{
    bool IsEquals(float x, float y);

    int DigitsInIntPart(float value);
    
    // Округляет число с плавающей точкой. numDigits - полное число знаков, по которым производится округление.
    // Например, RoundFloat(12.37137, 4) округлит до 12.40)
    float RoundFloat(float value, int numDigits);

    template<class T> T Abs(T x);
    
    int Pow10(int pow);

    int Sign(int val);

    template<class T> void CircleIncrease(T *value, T min, T max);

    template<class T> void CircleDecrease(T *value, T min, T max);

    template<class T> T Min(T x1, T x2)  { return (x1 < x2) ? x1 : x2;  }
};
