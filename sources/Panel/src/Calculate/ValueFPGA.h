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

    // ���������� ������� ��� ������ ��������� ��� ������� �������. order == 3 - ����, order == -3 - �����
    char *GetSuffixUnit(int order) const;

    int NumDigitsInNumber(uint number) const;
};
