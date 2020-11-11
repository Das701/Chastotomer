#pragma once
#include "defines.h"
#include "Display/Colors.h"
#include "Keyboard/Keyboard.h"
#include "Utils/Observer.h"


struct Enumeration
{
    uint8  value;           // ������� ��������� ������������
    char **names;
    char **ugo;
    const bool *correct;    // ���� ��������� �� ���� ������ �� ����� nullptr, �� ��� �������� ������������ ��������� �������.
                            // value �� ����� �������� ��������, ��������������� false
    int numStates;
    explicit Enumeration(uint8 v, const bool *_correct = nullptr, int states = 0) : value(v), names(nullptr), ugo(nullptr), correct(_correct), numStates(states) {}
    operator int()         { return (int)value; }
    char *ToText() const;
    char *UGO() const;
    int NumStates() const;
    bool Is(uint8 v) const { return value == v; }
};



class Item
{
    friend class Hint;
public:
    static const int HEIGHT = 35;

    // ������� ���������
    virtual void Draw(int x, int y, int width, bool selected = false) = 0;
    // ������� ��������� ������� ������/�������� �����
    virtual bool OnControl(const Control &) { return false; };

    static Color ColorBackground(bool selected);

    static Color ColorDraw(bool selected);

protected:
    // ����� ����� ��������� ��� ������� �����
    char *hint;

private:
    // ������� ��������� ��� �����
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
    char        *text;              // ������� �� �������������
    void       (*funcOnPress)();    // ��� ������� ���������� ����� ��������� ��������� �������������
    Enumeration *state;             // ����� ���������� � ���������� �������������
    virtual void CreateHint(char buffer[100]) const;
    // ����������� � ��������� ���������
    void  NextChoice();
};



class Page : public Item, public Observer
{
public:
    Page(Item **_items, void (*_onEvent)(EventType::E)) : selectedItem(0), items(_items), onEvent(_onEvent) {};

    virtual void Draw(int x, int y, int width, bool selected = false);
    virtual bool OnControl(const Control &) { return false; };

    // ���������� ��������� �� ���������� ����� ����
    Item *SelectedItem() { return items[selectedItem]; };

    // ���������� true, ���� ��� �������� �������
    bool IsPageModes() const;

    // ���������� true, ���� ��� �������� ��������
    bool IsPageSettings() const;

    // ������ ������� ��������� �������
    void SelectNextItem();

    // ������ ������� ���������� �������
    void SelectPrevItem();

    // ��������� �� ������������ ����� ����������� �����. ���� �� ������, ��� ���������� ������ - ���������������
    void VerifySelectedItem();

    virtual void OnEvent(EventType::E);

    // ����� ���������� �����
    int selectedItem;

private:

    // ���������� ���������� ������ �� ��������
    int NumItems() const;

    virtual void CreateHint(char buffer[100]) const { buffer[0] = 0; };

    // ���������� ������ �������� ���� � ������� num
    int WidthItem(int num) const;

    // ��������� �� ������ ��������� ����. ������������� ����.
    Item **items;

    void (*onEvent)(EventType::E);
};
