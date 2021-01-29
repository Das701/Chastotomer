#pragma once
#include "Utils/String.h"
#include "Utils/ValueSTRICT.h"


struct ValueFPGA
{
    ValueFPGA();
    virtual ~ValueFPGA() {};

    String value;


protected:

    String mainUnits;

    void SetValue(ValueSTRICT value, uint counter, uint add = 0);

    // Возвращает суффикс для единиц измерения для данного порядка. order == 3 - кило, order == -3 - милли
    char *GetSuffixUnit(int order) const;

    int NumDigitsInNumber(uint number) const;
};
