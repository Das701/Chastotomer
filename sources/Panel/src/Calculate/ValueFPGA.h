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

    // ���������� ������� ��� ������ ��������� ��� ������� �������. order == 3 - ����, order == -3 - �����
    virtual char *GetSuffixUnit(int order) const;

    // ���������� ������������� ������� ��� ��������� order
    static char *GetSuffixUnitRelated(int order);
};
