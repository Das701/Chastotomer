#pragma once
#include "Display/Symbols.h"
#include "Utils/Stack.h"
#include "Utils/String.h"
#include "Utils/ValueSTRICT.h"
#include <cstring>


struct ValueFPGA
{
    ValueFPGA() {};
    virtual ~ValueFPGA() {};

    static void Create(uint value1, uint value2 = 0U, uint value3 = 0U, uint value4 = 0U, uint value5 = 0U);

    static String GiveDigits();

    static String GiveUnits();

    static char *GiveStringValue();

    static void SetInvalidData();

    // Возвращает true, если значение - данные. Даже непринятые. Т.е. нет переполнения либо деления на нуль
    static bool IsData() { return !(IsDivNULL() || IsOverlapped()); }

    static bool IsDivNULL()     { return std::strcmp(GiveDigits().c_str(), UGO::DivNULL) == 0; }

    static bool IsOverlapped()  { return std::strcmp(GiveDigits().c_str(), UGO::OVERLAPPED) == 0; };

    static bool IsEmpty()       { return std::strcmp(GiveDigits().c_str(), UGO::EMPTY_VALUE) == 0; }

protected:

    String mainUnits;

    // Если (isOrdered == true), то приводить число к оптимальному порядку, иначе - выводить как есть
    void SetValue(ValueSTRICT value, uint counter, bool isOrdered = true);

    void SetValue(char *formt, ...);

    // Установить бесконечное значение - деление на ноль
    void SetInfiniteValue();

    void SetIntegerValue(uint value);

    int NumDigitsInNumber(uint number) const;

    // Возвращает суффикс для единиц измерения для данного порядка. order == 3 - кило, order == -3 - милли
    virtual char *GetSuffixUnit(int order) const;

    // Возвращает относительный порядок для заданного order
    static char *GetSuffixUnitRelated(int order);

private:

    static ValueFPGA *valueFPGA;

    static String value;
};


struct ValueFrequency : public ValueFPGA
{
    ValueFrequency();
};


struct ValueFrequency_Frequency : public ValueFrequency
{
    ValueFrequency_Frequency(uint counter1);
};


struct ValueFrequency_T_1 : public ValueFrequency
{
    ValueFrequency_T_1(uint counter1);
};


struct ValueFrequency_Ratio : public ValueFrequency
{
    ValueFrequency_Ratio(uint counter1, uint counter2);

protected:

    virtual char *GetSuffixUnit(int order) const;
};


struct ValueFrequency_Tachometer : public ValueFrequency
{
    ValueFrequency_Tachometer(uint counter1);
};


struct ValueFrequency_Comparator : public ValueFrequency
{
    ValueFrequency_Comparator(uint counter, int interpol1, int cal1, int interpol2, int cal2);

    class Stack : public ::Stack<double>
    {
    public:
        Stack(int size) : ::Stack<double>(size) {};
        bool AppendValue(double value);
        double GetFromEnd(int fromEnd);
    private:
    };

    static Stack values;
};


struct ValuePeriod : public ValueFPGA
{
    ValuePeriod();
};


struct ValuePeriod_Period : public ValuePeriod
{
    ValuePeriod_Period(uint counter1);
};


struct ValuePeriod_F_1 : public ValuePeriod
{
    ValuePeriod_F_1(uint counter1);
};


struct ValueDuration : public ValueFPGA
{
    ValueDuration();
};


struct ValueDuration_Ndt_StartStop : public ValueDuration
{
    ValueDuration_Ndt_StartStop(uint counter);
};


struct ValueDuration_Ndt_1ns : public ValueDuration
{
    ValueDuration_Ndt_1ns(uint timer, uint cal1, uint cal2);
};


struct ValueDuration_Phase_FillFactor : public ValueDuration
{
    ValueDuration_Phase_FillFactor(uint period, uint duration);

protected:

    virtual char *GetSuffixUnit(int order) const;
};


struct ValueCountPulse : public ValueFPGA
{
    ValueCountPulse(uint counter);
};
