#pragma once
#include "defines.h"
#include "Keyboard/Keyboard.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Item
{
public:
    /// ������� ���������
    virtual void Draw(int x, int y) = 0;
    /// ������� ��������� ������� ������/�������� �����
    virtual bool OnControl(Control control) { return false; };
    
    static const int WIDTH = 35;
    static const int HEIGHT = 11;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Button : public Item
{
public:
    Button(char *_text) : text(_text){};

    virtual void Draw(int x, int y);
    virtual bool OnControl(Control control);
    
private:
    char *text;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Switch : public Item
{
public:

    Switch(char *_text, uint8 *_state, uint8 _num, void(*_onClick)()) : text(_text), funcOnPress(_onClick), state(_state), num(_num) {};

    virtual void Draw(int x, int y);
    virtual bool OnControl(Control control);

private:
    char  *text;            ///< ������� �� �������������
    void (*funcOnPress)();  ///< ��� ������� ���������� ����� ��������� ��������� �������������
    uint8 *state;           ///< ����� ���������� � ���������� �������������
    uint8  num;             ///< ���������� ��������� �������������
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Page : public Item
{
public:
    Page(Item **_items = nullptr) : items(_items), selectedItem(0) {};

    virtual void Draw(int x, int y);
    virtual bool OnControl(Control control);    
    /// ���������� ��������� �� ���������� ����� ����
    Item *SelectedItem() { return items[selectedItem]; };

    static Page empty;
    
private:
    /// ������ ������� ��������� �������
    void SelectNextItem();
    /// ������ ������� ���������� �������
    void SelectPrevItem();
    /// ���������� ���������� ������ �� ��������
    int NumItems();
    /// ��������� �� ������ ��������� ����. ������������� ����.
    Item **items;
    int selectedItem;
    
};
