#pragma once
#include "Utils/String.h"


struct ValueFPGA
{
    ValueFPGA();
    virtual ~ValueFPGA() {};

    String value;

    String mainUnits;

    // ���������� ������� ��� ������ ��������� ��� ������� �������. order == 3 - ����, order == -3 - �����
    char *GetSuffixUnit(int order) const;

    int NumDigitsInNumber(uint number) const;
};
