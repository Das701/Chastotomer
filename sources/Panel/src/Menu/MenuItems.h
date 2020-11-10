#pragma once
#include "defines.h"
#include "Display/Colors.h"
#include "Keyboard/Keyboard.h"
#include "Utils/Observer.h"


struct Enumeration
{
    // Текущее состояние перечисления
    uint8  value;
    char **names;
    char **ugo;
    Enumeration(uint8 v) : value(v), names(nullptr), ugo(nullptr) {}
    operator int() { return (int)value; }
    char *ToText() const { return names[value]; }
    char *UGO() const { return ugo[value]; }
    int NumStates() const;
    bool Is(uint8 v) const { return value == v; }
};



class Item
{
    friend class Hint;
public:
    static const int HEIGHT = 35;

    // Функция отрисовки
    virtual void Draw(int x, int y, int width, bool selected = false) = 0;
    // Функция обработки нажатия кнопки/поворота ручки
    virtual bool OnControl(const Control &) { return false; };

    static Color ColorBackground(bool selected);

    static Color ColorDraw(bool selected);

protected:
    // Общая часть подсказки для данного итема
    char *hint;

private:
    // Создать подсказку для итема
    virtual void CreateHint(char buffer[100]) const = 0;
};



class Button : public Item
{
public:
    Button(char *_text, void (*funcPress)()) : text(_text), funcOnPress(funcPress) {};

    virtual void Draw(int x, int y, int width, bool selected = false);
    virtual bool OnControl(const Control &control);
    
private:
    char *text;
    void (*funcOnPress)();

    virtual void CreateHint(char [100]) const {};
};



class Switch : public Item
{
public:

    Switch(char *_text, char *_hint, char **_names, char **_ugo, Enumeration *_state, void(*_onClick)()) :
        text(_text), funcOnPress(_onClick), state(_state)
    {
        state->names = _names;
        state->ugo = _ugo;
        hint = _hint;
    };
    virtual void Draw(int x, int y, int width, bool selected = false);
    virtual bool OnControl(const Control &control);

private:
    char        *text;              // Надпись на переключателе
    void       (*funcOnPress)();    // Эта функция вызывается после изменения состояния переключателя
    Enumeration *state;             // Адрес переменной с состоянием переключателя
    virtual void CreateHint(char buffer[100]) const;
};



class Page : public Item, public Observer
{
public:
    Page(Item **_items, void (*_onEvent)(EventType::E)) : selectedItem(0), items(_items), onEvent(_onEvent) {};

    virtual void Draw(int x, int y, int width, bool selected = false);
    virtual bool OnControl(const Control &) { return false; };

    // Возвращает указатель на выделенный пункт меню
    Item *SelectedItem() { return items[selectedItem]; };

    // Возвращает true, если это страница режимов
    bool IsPageModes() const;

    // Возвращает true, если это страница настроек
    bool IsPageSettings() const;

    // Делает текущим следующий элемент
    void SelectNextItem();

    // Делает текущим предыдущий элемент
    void SelectPrevItem();

    // Проверить на корректность номер выделенного итема. Если он больше, чем количество итемов - скорректировать
    void VerifySelectedItem();

    virtual void OnEvent(EventType::E);

    // Номер выбранного итема
    int selectedItem;

private:

    // Возвращает количество итемов на странице
    int NumItems() const;

    virtual void CreateHint(char buffer[100]) const { buffer[0] = 0; };

    // Возвращает ширину элемента меню с номером num
    int WidthItem(int num) const;

    // Указатель на массив элементов меню. Заканчивается нулём.
    Item **items;

    void (*onEvent)(EventType::E);
};
