#pragma once
#include "Utils/String.h"


class Item;


class Hint
{
public:

    // ������ ��������� ��� item
    static void Create(const Item *item);

    // ���������� ����� ���������
    static String Text();

    // ���������� ����, ��� �������� ��������� ���������
    static const Item *UnderItem();

    // �������� ���������
    static void Hide();
};
