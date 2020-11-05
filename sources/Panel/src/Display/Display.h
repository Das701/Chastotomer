#pragma once
#include "Display/Colors.h"


namespace Display
{
	static const int WIDTH = 480;
	static const int HEIGHT = 272;

    // Число частей, на которые поделен дисплей для отрисовки
    static const int NUM_PARTS = 2;

    void Init();

    // Вызвать, если требуется перерисовка
    void Refresh();

    void Update();

    void BeginScene();

    void EndScene();

    bool DrawWelcomeScreen();

    // Номер верхней строки в текущей отрисовке.
    // Отрисовка происходит в два этапа - сначала верхняя часть (TopRow() == 0), затем нижняя (TopRow() == Display/HEIGHT / 2)
    int TopRow();
};
