#pragma once
#include "Display/Colors.h"


struct Display
{
	static const int WIDTH = 480;
	static const int HEIGHT = 272;

    // Число частей, на которые поделен дисплей для отрисовки
    static const int NUM_PARTS = 2;

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

private:
    static bool sendToSCPI;     // Если true, то надо посылать в SCPI

    static void InitHardware();
};
