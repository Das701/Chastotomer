#pragma once


class Item;


class Hint
{
public:
    /// ������ ��������� ��� item
    static void Create(const Item *item);
    /// ���������� ����� ���������
    static char *Text();
    /// ���������� ����, ��� �������� ��������� ���������
    static const Item *UnderItem();
    /// �������� ���������
    static void Hide();
};
