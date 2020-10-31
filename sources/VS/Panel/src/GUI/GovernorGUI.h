#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class GovernorGUI : public wxPanel
{
public:
    // code - код ручки
    GovernorGUI(wxWindow *parent, const wxPoint &position);

private:
    static const int radius = 32;
    static const float stepDegree;
    
    void OnPaint(wxPaintEvent &);
    void OnMouseLeftDown(wxMouseEvent &);
    void OnMouseLeftUp(wxMouseEvent &);
    void OnMouseMove(wxMouseEvent &);
    void OnTimer(wxTimerEvent &);

    // Возвращает true, если курсор мыши находится над изображением ручки
    bool MouseOnGovernor(wxMouseEvent &);

    // Синус от градусов
    float Sin(float);

    // Косинус от градусов
    float Cos(float);

    // Эта фунция вызывается при переключении ручки в следующую/предыдущую позицию
    void FuncRotate(int delta);

    // Эта функция вызываетс при нажатии ручки
    void FuncPress();

    struct StructCursor
    {
        bool leftIsDown;        // true, если левая кнопка нажата
        POINT position;         // Позиция курсора
        int state;              // Состояние VK_LBUTTON

        // Рассчитывает dX и dY между position и текущей позицией
        int CalculateDelta();

        // Возвращает true, если сейчас нажата левая кнопка мыши
        bool LeftIsDown();

        // Вызывается при нажатии левой кнопки мыши
        void OnPressLeftButton();
    } cursor;

    float angleFull = 0.0F;     // Реальный угол поворота ручки (без градаций переключения)
    float angleDiscrete = 0.0F; // Отображаемый угол поворота ручки (у учётом градаций переключения)

    wxTimer timer;
    
    bool needEventPress = true; // Если true, то при отпускании мыши нужно посылать событие нажатия ручки
};
