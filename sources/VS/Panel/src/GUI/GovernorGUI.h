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
    // ���� �������� �����
    float angle = 0.0F;
    // ����� ������� �������� ������������� �����
    const int stepAngle = 60;

    wxTimer timer;

    struct StructCursor
    {
        // true, ���� ����� ������ ������
        bool leftIsDown;
        // ������� �������
        POINT position;
        // ��������� VK_LBUTTON
        int state;
        // ������������ dX � dY ����� position � ������� ��������
        int CalculateDelta();
        // ���������� true, ���� ������ ������ ����� ������ ����
        bool LeftIsDown();
        // ���������� ��� ������� ����� ������ ����
        void OnPressLeftButton();
    } cursor;

    void OnPaint(wxPaintEvent &);
    void OnMouseLeftDown(wxMouseEvent &);
    void OnMouseMove(wxMouseEvent &);
    void OnTimer(wxTimerEvent &);

    // ���������� true, ���� ������ ���� ��������� ��� ������������ �����
    bool MouseOnGovernor(wxMouseEvent &);

    float Sin(int grad);
    float Cos(int grad);
};
