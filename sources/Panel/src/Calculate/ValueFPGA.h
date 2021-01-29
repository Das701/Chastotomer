#pragma once
#include "Utils/String.h"


struct ValueFPGA
{
    ValueFPGA();
    virtual ~ValueFPGA() {};

    String value;

    String mainUnits;

    // Возвращает суффикс для единиц измерения для данного порядка. order == 3 - кило, order == -3 - милли
    char *GetSuffixUnit(int order) const;

    int NumDigitsInNumber(uint number) const;
};
