#pragma once
#include "Utils/String.h"


struct ValueFPGA
{
    ValueFPGA(uint counter1, uint counter2 = 0, uint counter3 = 0, uint counter4 = 0);
    virtual ~ValueFPGA() {};

    String value;

    String mainUnits;

    // Возвращает суффикс для единиц измерения для данного порядка. order == 3 - кило, order == -3 - милли
    virtual char *GetSuffixUnit(int order) { (void)order; return ""; };

    int NumDigitsInNumber(uint number);
};
