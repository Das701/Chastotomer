#pragma once
#include "defines.h"
#include "Keyboard/Keyboard.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Item
{
public:
    /// Функция отрисовки
    virtual void Draw(int x, int y) {};
    /// Функция обработки нажатия кнопки/поворота ручки
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
    /// Делает текущим следующий элемент
    void SelectNextItem();
    /// Делает текущим предыдущий элемент
    void SelectPrevItem();
    /// Возвращает количество итемов на странице
    int NumItems();
    /// Указатель на массив элементов меню. Заканчивается нулём.
    Item **items;
    int currentItem;
    int clickCount;
};
