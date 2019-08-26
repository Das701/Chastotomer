#pragma once
#include "defines.h"
#include "Keyboard/Keyboard.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Item
{
public:
    /// ������� ���������
    virtual void Draw(int x, int y) {};
    /// ������� ��������� ������� ������/�������� �����
    virtual bool OnControl(Control control) { return false; };
    
    virtual void DrawOnClick() {};
    
    static const int WIDTH = 35;
    static const int HEIGHT = 11;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Button : public Item
{
public:
    Button(char *_text) : text(_text) {};

    virtual void Draw(int x, int y);
    
    virtual bool OnControl(Control control);

        
private:
    char *text;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Page : public Item
{
public:
    Page(Item **_items = nullptr) : items(_items), currentItem(0), clickCount(0) {};
    
    virtual void Draw(int x, int y);
    
    virtual bool OnControl(Control control);    
     
    virtual void DrawOnClick();
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
    int currentItem;
    int clickCount;
};
