#pragma once
#include "Display/Colors.h"


namespace Display
{
	static const int WIDTH = 480;
	static const int HEIGHT = 272;

    void Init();

    // �������, ���� ��������� �����������
    void Refresh();

    void Update();

    void BeginScene();

    void EndScene();

    void DrawWelcomeScreen();

    // ����� ������� ������ � ������� ���������.
    // ��������� ���������� � ��� ����� - ������� ������� ����� (TopRow() == 0), ����� ������ (TopRow() == Display/HEIGHT / 2)
    int TopRow();
};
