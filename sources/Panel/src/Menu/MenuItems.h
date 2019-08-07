#pragma once
#include "defines.h"


class Control
{
public:
    static const int WIDTH = 25;
    static const int HEIGHT = 10;
    static const int D_X = 2;

    virtual void Draw(int, int) {}
};


class Page : public Control
{
public:
    static const int CONTROLS_ON_PAGE = 6;

    virtual void Draw(int x, int y);

    void Init(Control **_controls, pFuncVII funcDrawClosed) { controls = _controls; drawClosed = funcDrawClosed; }

    Page() : controls(nullptr), currentControl(0), currentSubPage(0), closed(true), drawClosed(nullptr), lastIndexed(0) {}

    void Open() { closed = false; };
    
private:
    int NumControls();
    /// Первый контрол на дисплее
    Control *FirstOnDisplay();
    /// Возвращает указатель на следующий контрол
    Control *NextOnDisplay();
    /// Последний контрол на дисплее
    Control *LastOnDisplay();

    void DrawOpened(int x, int y);

    void DrawClosed(int x, int y);
    /// Здесь контролы страницы. Последний равен 0.
    Control **controls;
    /// Текущий контрол
    int currentControl;
    /// Текущая подстраница
    int currentSubPage;
    /// Если true, страница закрыта
    bool closed;

    pFuncVII drawClosed;
    /// Индекс последнего элемента, возвращённого FirstOnDisplay() или NextOnDisplay()
    int lastIndexed;
};
