#pragma once
#include "Display/Colors.h"


namespace Display
{
	static const int WIDTH = 480;
	static const int HEIGHT = 272;

    // ����� ������, �� ������� ������� ������� ��� ���������
    static const int NUM_PARTS = 2;

    void Init();

    // �������, ���� ��������� �����������
    void Refresh();

    void Update();

    void BeginScene();

    void EndScene();

    bool DrawWelcomeScreen();

    // ����� ������� ������ � ������� ���������.
    // ��������� ���������� � ��� ����� - ������� ������� ����� (TopRow() == 0), ����� ������ (TopRow() == Display/HEIGHT / 2)
    int TopRow();
};
