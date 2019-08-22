#pragma once
#include "defines.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Item
{
public:
    virtual void Draw(int x, int y) {};

    static const int WIDTH = 35;
    static const int HEIGHT = 11;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Button : public Item
{
public:
    Button(char *_text) : text(_text) {};

    virtual void Draw(int x, int y);

private:
    char *text;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Page : public Item
{
public:
    Page(Item **_items = nullptr) : items(_items) {};

    virtual void Draw(int x, int y);

    static Page empty;

private:
    /// ��������� �� ������ ��������� ����. ������������� ����.
    Item **items;
};
