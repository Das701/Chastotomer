#pragma once


namespace Math
{
    bool IsEquals(float x, float y);

    int DigitsInIntPart(float value);
    
    // ��������� ����� � ��������� ������. numDigits - ������ ����� ������, �� ������� ������������ ����������.
    // ��������, RoundFloat(12.37137, 4) �������� �� 12.40)
    float RoundFloat(float value, int numDigits);

    template<class T> T    Abs(T x);
    
    int Pow10(int pow);

    template<class T> void CircleIncrease(T *value, T min, T max);

    template<class T> void CircleDecrease(T *value, T min, T max);
};
