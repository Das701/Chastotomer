#pragma once
#include "Display/Colors.h"


class DataZone;


struct Display
{
	static const int PHYSICAL_WIDTH = 480;
	static const int PHYSICAL_HEIGHT = 272;

    // Число частей, на которые поделен дисплей для отрисовки
    static const int NUM_PARTS = 2;

    static int Width();

    static int Height();

    static void Init();

    // Вызвать, если требуется перерисовка
    static void Refresh();

    static void Update();

    static void BeginScene();

    static void EndScene();

    static bool DrawWelcomeScreen();

    // Номер верхней строки в текущей отрисовке.
    // Отрисовка происходит в два этапа - сначала верхняя часть (TopRow() == 0), затем нижняя (TopRow() == Display/HEIGHT / 2)
    static int TopRow();

    // Возвращает true, если прямоугльник находится в отрисовываемой в данный момент времени части экрана
    static bool InDrawingPart(int y, int height);

    // Отрисовать очередную часть экрана
    static void DrawPartScreen(int num, bool debugInfo);

    static void SendToSCPI();

    static void Draw(uint *buffer);

    // Здесь отрисовка результата измерения
    static DataZone *zoneData;

    // Подготовить дисплей к непосредственному рисованию в аппаратуру
    static void Prepare(int width, int height);
    static void SendToFSMC(int x0, int y0);
    static void Restore();

private:
    static bool sendToSCPI;     // Если true, то надо посылать в SCPI

    static void InitHardware();

    static void DrawScreen();
};
