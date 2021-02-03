#pragma once
#include "Calculate/ValuesFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Stack.h"
#include "Utils/String.h"
#include "Utils/ValueSTRICT.h"


struct MathFPGA
{
    static void DecToBin(int dec, char *bin);
    static String BinToString(pString bin, int num);

    static int NA; //-V707
    static int NB; //-V707

    // ��������� �������� �� ���������� ������
    struct Validator
    {
        // ��������� �������� ����, ��� ��������� ���������� � ��� ���������� ������ (��� GiveData())
        static void SetInvalidData();

        // ��������� �������� ����, ��� ������ ����� ��������
        static void SetValidData();

        // ���������� true, ���� ������ �������
        static bool DataIsValid();

        // ���������� true, ���� ������ ������� ���� ������� ����� ���������� ��������� ��������
        static bool VerySmallTime() { return TIME_MS - timeClearedFlag < deltaTime; };

    private:

        static bool isEmpty;                // ������������� � true �������� ��������, ��� ������ ��� ����������� ���
        static uint timeClearedFlag;        // �����, ����� ������ ���� �������
        static const uint deltaTime = 200;  // ��� ����� ����� ������� ����� ����, ��� ���������� ���������
    };

    struct Measure
    {
        static void SetNewData(uint value1, uint value2, uint value3 = 0, uint value4 = 0, uint value5 = 0);

    };


    struct Auto
    {
        friend struct FPGA;

        static int Mid();
        static int Min();
        static int Max();

        static String Give();

        static void Refresh();

    private:
        static uint fpgaMin;
        static uint fpgaMid;
        static uint fpgaMax;
    };
};
