#pragma once
#include "defines.h"
#include "Keyboard/Keyboard.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Item
{
public:
    /// Функция отрисовки
    virtual void Draw(int x, int y) = 0;
    /// Функция обработки нажатия кнопки/поворота ручки
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
    char  *text;            ///< Надпись на переключателе
    void (*funcOnPress)();  ///< Эта функция вызывается после изменения состояния переключателя
    uint8 *state;           ///< Адрес переменной с состоянием переключателя
    uint8  num;             ///< Количество состояний переключателя
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Page : public Item
{
public:
    Page(Item **_items = nullptr) : items(_items), selectedItem(0) {};

    virtual void Draw(int x, int y);
    virtual bool OnControl(Control control);    
    /// Возвращает указатель на выделенный пункт меню
    Item *SelectedItem() { return items[selectedItem]; };

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
    int selectedItem;
    
};
