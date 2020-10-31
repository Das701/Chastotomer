#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class GovernorGUI : public wxPanel
{
public:
    GovernorGUI(wxWindow *parent, const wxPoint &position);

private:
    const int radius = 20;
    // Угол поворота ручки
    float angle = 0.0F;
    // Через столько градусов переключается ручка
    const int stepAngle = 60;

    wxTimer timer;

    struct StructCursor
    {
        // true, если левая кнопка нажата
        bool leftIsDown;
        // Позиция курсора
        POINT position;
        // Состояние VK_LBUTTON
        int state;
        // Рассчитывает dX и dY между position и текущей позицией
        int CalculateDelta();
        // Возвращает true, если сейчас нажата левая кнопка мыши
        bool LeftIsDown();
        // Вызывается при нажатии левой кнопки мыши
        void OnPressLeftButton();
    } cursor;

    void OnPaint(wxPaintEvent &);
    void OnMouseLeftDown(wxMouseEvent &);
    void OnMouseMove(wxMouseEvent &);
    void OnTimer(wxTimerEvent &);

    // Возвращает true, если курсор мыши находится над изображением ручки
    bool MouseOnGovernor(wxMouseEvent &);

    float Sin(int grad);
    float Cos(int grad);
};
