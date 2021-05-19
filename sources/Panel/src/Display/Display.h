#pragma once
#include "Display/Colors.h"


class DataZone;
class ProgressBarTimeMeasureZone;


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

    static void BeginScene(int x = -1, int y = -1);

    static void EndScene();

    static void DrawWelcomeScreen();
    static void DrawKeyboardFailScreen();

    // ����� ������� ������ � ������� ���������.
    // ��������� ���������� � ��� ����� - ������� ������� ����� (TopRow() == 0),
    // ����� ������ (TopRow() == Display/HEIGHT / 2)
    static int TopRow();

    // ���������� true, ���� ������������ ��������� � �������������� � ������ ������ ������� ����� ������
    static bool InDrawingPart(int y, int height);

    // ���������� ��������� ����� ������
    static void DrawPartScreen(int num, bool debugInfo);

    static void SendToSCPI();

    // ����� ��������� ���������� ���������
    static DataZone *zoneData;

    // ��������� ������� ������� ������� ���������
    static ProgressBarTimeMeasureZone *zoneProgressBarTimeMeasure;


    // ��������� ��� ���������������� ��������� � �������
    struct Sender
    {
        // ����������� ������� � ����������������� ��������� � ����������
        static void Prepare(int width, int height);

        // ������� � ������� �������, ������������ (x0, y0) � ����� ���������� (width, height)
        static void SendToFSMC(int x0, int y0);

        // ������������ � ����������� ���������.
        static void Restore();
    };

    // ��� ������� ������ ��� win-������
    static void Draw(const uint *buffer);

private:
    static bool sendToSCPI;     // ���� true, �� ���� �������� � SCPI
    static bool drawingScene;   // ���� true - ��������� � ��������� ��������� �������� ����� - ����� BeginScene()
                                // � EndScene()

    static void InitHardware();

    static void DrawScreen();
};
