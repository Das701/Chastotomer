#pragma once
#include "Utils/Stack.h"
#include "Utils/String.h"
#include "Utils/ValueSTRICT.h"


struct ValueFPGA
{
    ValueFPGA();
    virtual ~ValueFPGA() {};

    String value;


protected:

    String mainUnits;

    void SetValue(ValueSTRICT value, uint counter);

    void SetIntegerValue(uint value);

    int NumDigitsInNumber(uint number) const;

    // Возвращает суффикс для единиц измерения для данного порядка. order == 3 - кило, order == -3 - милли
    virtual char *GetSuffixUnit(int order) const;

    // Возвращает относительный порядок для заданного order
    static char *GetSuffixUnitRelated(int order);
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

protected:

    virtual char *GetSuffixUnit(int order) const;
};
