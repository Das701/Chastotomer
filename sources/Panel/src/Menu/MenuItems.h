#pragma once
#include "defines.h"
#include "Keyboard/Keyboard.h"


struct Enumeration
{
    /// Текущее состояние перечисления
    uint8  value;
    char **names;
    char **ugo;
    Enumeration(uint8 v) : value(v) {}
    operator int() { return (int)value; }
    char *ToText() const { return names[value]; }
    char *UGO() const { return ugo[value]; }
    int NumStates() const;
    bool Is(uint8 v) const { return value == v; }

};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Item
{
public:
    /// Функция отрисовки
    virtual void Draw(int x, int y) = 0;
    /// Функция обработки нажатия кнопки/поворота ручки
    virtual bool OnControl(const Control &control) { return false; };

    static const int WIDTH = 35;
    static const int HEIGHT = 11;
    /// Здесь хранится полная подсказка для последнего использованного итема
    static char hint[100];

protected:
    /// Общая часть подсказки
    char *commonHint;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Button : public Item
{
public:
    Button(char *_text) : text(_text){};

    virtual void Draw(int x, int y);
    virtual bool OnControl(const Control &control);
    
private:
    char *text;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Switch : public Item
{
public:

    Switch(char *_text, char *_comHint, char **_names, char **_ugo, Enumeration *_state, void(*_onClick)()) :
        text(_text), funcOnPress(_onClick), state(_state)
    {
        state->names = _names;
        state->ugo = _ugo;
        commonHint = _comHint;
    };
    virtual void Draw(int x, int y);
    virtual bool OnControl(const Control &control);

private:
    char        *text;              ///< Надпись на переключателе
    void       (*funcOnPress)();    ///< Эта функция вызывается после изменения состояния переключателя
    Enumeration *state;             ///< Адрес переменной с состоянием переключателя

    void CreateHint();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Page : public Item
{
public:
    Page(Item **_items = nullptr) : items(_items), selectedItem(0) {};

    virtual void Draw(int x, int y);
    virtual bool OnControl(const Control &control);    
    /// Возвращает указатель на выделенный пункт меню
    Item *SelectedItem() { return items[selectedItem]; };

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
