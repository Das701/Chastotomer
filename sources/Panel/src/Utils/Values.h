#pragma once
#include "Utils/String.h"
#include <limits>



class Hex
{
public:
    Hex(uint v) : value(v) {};
    
    // ����������� �������� � ��������� ������ � ����������������� ����. depth ����� ����������� ����� - 8, 16 ��� 32 ���.
    char* ToHex(int depth, char bufferOut[9]) const;
    
    // ����������� �������� � ��������� ������ � �������� ����. depth ����� ����������� ����� - 8, 16 ��� 32 ���.
    char* ToBin(int depth, char bufferOut[36]) const;
    
    // ���������� ������ � ������� pos �� ���������� (��������)
    char DigitInPosition(int pos) const;

    void Set(uint v) { value = v; };
    
    // ���������� ����� ���������� ��������.
    int NumDigits() const;

    operator uint() const { return value; };

private:

    char *BinToString8(uint8 value, char buffer[9]) const;

    uint value;
};


class Integer
{
public:

    Integer(int v) : value(v) {};

    int NumDigits() const;

    String ToString(bool alwaysSign, int numMinFields = 1) const;

    static const int ERROR = 0x7fffffff;

    int operator[](int n);

private:

    int value;
};


class Int16
{
public:
    static const uint16 ERROR = 0x7fff;
};


struct Uint8
{
    static const uint8 ERROR = 255;
};


class Frequency
{
public:
    Frequency(float v) : value(v) {};
    
    // ����������� freq ���� � ��������� ������. ��� ���� ����� ���������� ���� � ������ ����� numDigits
    char* ToStringAccuracy(char bufferOut[20], int numDigits) const;

    String ToString() const;

private:

    float value;
};


class Time
{
public:

    explicit Time(float v) : value(v) {};
    // ����������� time ������ � ��������� ������
    char* ToStringAccuracy(bool alwaysSign,      // ���� true, ���� ������������ ���� ���� time > 0
                           char bufferOut[20],   // ���� ������������ ������������ ��������
                           int numDigits         // ���������� ���� � ������
    ) const;

    String ToString(bool alwaysSign) const;

    float Value() const { return value; }

private:
    float value;
};


class Voltage
{
public:

    Voltage(float v) : value(v) {};

    String ToString(bool alwaysSign) const;

private:

    float value;
};


class Current
{
public:
    Current(float v) : value(v) {};

    String ToString() const;

private:
    char *ToString(char buffer[50]) const;

    float value;
};


class Phase
{
public:

    Phase(float v) : value(v) {};

    char* ToString(char buffer[20]) const;

private:

    float value;
};


class Float
{
public:

    Float(float v);

    String ToString(bool alwaysSign, int numDigits) const;

    static const float ERROR;

private:

    float value;
};
