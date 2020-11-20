#pragma once
#include "Display/Colors.h"


class DataZone;


struct Display
{
	static const int PHYSICAL_WIDTH = 480;
	static const int PHYSICAL_HEIGHT = 272;

    // ����� ������, �� ������� ������� ������� ��� ���������
    static const int NUM_PARTS = 2;

    static int Width();

    static int Height();

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

    // ����� ��������� ���������� ���������
    static DataZone *zoneData;

    // ����������� ������� � ����������������� ��������� � ����������
    static void Prepare(int width, int height);
    static void SendToFSMC(int x0, int y0);
    static void Restore();

private:
    static bool sendToSCPI;     // ���� true, �� ���� �������� � SCPI

    static void InitHardware();

    static void DrawScreen();
};
