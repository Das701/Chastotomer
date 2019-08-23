#pragma once
#include "defines.h"
#include "Keyboard/Keyboard.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Item
{
public:
    virtual void Draw(int x, int y) {};
    
    virtual bool PressButton(Control control) { return false; };

    static const int WIDTH = 35;
    static const int HEIGHT = 11;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Button : public Item
{
public:
    Button(char *_text) : text(_text) {};

    virtual void Draw(int x, int y);

    //virtual bool PressButton(Control control);    
        
private:
    char *text;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Page : public Item
{
public:
    Page(Item **_items = nullptr) : items(_items), currentItem(0) {};
    
    int NumItems();
        
    void ControlRight();
    void ControlLeft();
        
    virtual void Draw(int x, int y);
    
    virtual bool PressButton(Control control);    
        
    static Page empty;

private:
    /// Указатель на массив элементов меню. Заканчивается нулём.
    Item **items;
    int currentItem;
};
