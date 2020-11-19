#pragma once
#include "Display/Colors.h"


struct Display
{
	static const int WIDTH = 480;
	static const int HEIGHT = 272;

    // ����� ������, �� ������� ������� ������� ��� ���������
    static const int NUM_PARTS = 2;

    static void Init();

    // �������, ���� ��������� �����������
    static void Refresh();

    static void Update();

    static void BeginScene();

    static void EndScene();

    static bool DrawWelcomeScreen();

    // ����� ������� ������ � ������� ���������.
    // ��������� ���������� � ��� ����� - ������� ������� ����� (TopRow() == 0), ����� ������ (TopRow() == Display/HEIGHT / 2)
    static int TopRow();

    // ���������� true, ���� ������������ ��������� � �������������� � ������ ������ ������� ����� ������
    static bool InDrawingPart(int y, int height);

    // ���������� ��������� ����� ������
    static void DrawPartScreen(int num, bool debugInfo);

    static void SendToSCPI();

    static void Draw(uint *buffer);

private:
    static bool sendToSCPI;     // ���� true, �� ���� �������� � SCPI

    static void InitHardware();
};
