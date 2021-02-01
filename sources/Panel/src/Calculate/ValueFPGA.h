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

    void SetValue(ValueSTRICT value, uint counter);

    int NumDigitsInNumber(uint number) const;

    // Возвращает суффикс для единиц измерения для данного порядка. order == 3 - кило, order == -3 - милли
    virtual char *GetSuffixUnit(int order) const;

    // Возвращает относительный порядок для заданного order
    static char *GetSuffixUnitRelated(int order);
};
