#pragma once


namespace SCPI
{
    struct Sender
    {
        // ����� ���������� ���������� ����������� �������� ValueFPGA ������ �������� ��� �������
        static void SendValue(pchar data);
    private:
        // ���� true, �� ������ ��������� � �������
        static bool needSend;
    };
}
