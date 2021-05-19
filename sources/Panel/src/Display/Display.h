#pragma once
#include "Display/Colors.h"


class DataZone;
class ProgressBarTimeMeasureZone;


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

    static void BeginScene(int x = -1, int y = -1);

    static void EndScene();

    static void DrawWelcomeScreen();
    static void DrawKeyboardFailScreen();

    // Номер верхней строки в текущей отрисовке.
    // Отрисовка происходит в два этапа - сначала верхняя часть (TopRow() == 0),
    // затем нижняя (TopRow() == Display/HEIGHT / 2)
    static int TopRow();

    // Возвращает true, если прямоугльник находится в отрисовываемой в данный момент времени части экрана
    static bool InDrawingPart(int y, int height);

    // Отрисовать очередную часть экрана
    static void DrawPartScreen(int num, bool debugInfo);

    static void SendToSCPI();

    // Здесь отрисовка результата измерения
    static DataZone *zoneData;

    // Отрисовка времени отсчёта времени измерения
    static ProgressBarTimeMeasureZone *zoneProgressBarTimeMeasure;


    // Структура для непосредсвенного рисования в дисплей
    struct Sender
    {
        // Подготовить дисплей к непосредственному рисованию в аппаратуру
        static void Prepare(int width, int height);

        // Послать в дисплей область, ограниченную (x0, y0) и ранее засланными (width, height)
        static void SendToFSMC(int x0, int y0);

        // Восстановить к нормальному рисованию.
        static void Restore();
    };

    // Эта функция только для win-версии
    static void Draw(const uint *buffer);

private:
    static bool sendToSCPI;     // Если true, то надо посылать в SCPI
    static bool drawingScene;   // Если true - находимся в состоянии отрисовки основной части - между BeginScene()
                                // и EndScene()

    static void InitHardware();

    static void DrawScreen();
};
