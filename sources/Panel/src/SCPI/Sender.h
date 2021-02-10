#pragma once


namespace SCPI
{
    struct Sender
    {
        struct Mode
        {
            enum E
            {
                Hand,
                Auto,
                Count
            };
        };

        // ����� ���������� ���������� ����������� �������� ValueFPGA ������ �������� ��� �������
        static void SendValue(pchar data);

        // ��������� ������� ��� ��������������� ������ ������� ������
        static void SetMode(Mode::E m);

        // ����������� ������� � ���� n ��������� ��������
        static void SendArray(int n);

    private:

        static int valuesForSend;

        static Mode::E mode;
    };
}