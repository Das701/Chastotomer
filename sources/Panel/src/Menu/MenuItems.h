#pragma once
#include "defines.h"
#include "Keyboard/Keyboard.h"


struct Enumeration
{
    /// ������� ��������� ������������
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Item
{
    friend class Hint;
public:
    static const int WIDTH = 51;
    static const int HEIGHT = 30;

    /// ������� ���������
    virtual void Draw(int x, int y, bool selected = false) = 0;
    /// ������� ��������� ������� ������/�������� �����
    virtual bool OnControl(const Control &) { return false; };

protected:
    /// ����� ����� ��������� ��� ������� �����
    char *hint;

private:
    /// ������� ��������� ��� �����
    virtual void CreateHint(char buffer[100]) const = 0;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Button : public Item
{
public:
    Button(char *_text) : text(_text){};

    virtual void Draw(int x, int y, bool selected = false);
    virtual bool OnControl(const Control &control);
    
private:
    char *text;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    virtual void Draw(int x, int y, bool selected = false);
    virtual bool OnControl(const Control &control);

private:
    char        *text;              ///< ������� �� �������������
    void       (*funcOnPress)();    ///< ��� ������� ���������� ����� ��������� ��������� �������������
    Enumeration *state;             ///< ����� ���������� � ���������� �������������
    virtual void CreateHint(char buffer[100]) const;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Page : public Item
{
public:
    Page(Item **_items = nullptr) : items(_items), selectedItem(0) {};

    virtual void Draw(int x, int y, bool selected = false);
    virtual bool OnControl(const Control &control);    
    /// ���������� ��������� �� ���������� ����� ����
    Item *SelectedItem() { return items[selectedItem]; };

private:
    /// ������ ������� ��������� �������
    void SelectNextItem();
    /// ������ ������� ���������� �������
    void SelectPrevItem();
    /// ���������� ���������� ������ �� ��������
    int NumItems();
    /// ��������� �� ������ ��������� ����. ������������� ����.
    Item **items;
    /// ����� ���������� �����
    int selectedItem;

    virtual void CreateHint(char buffer[100]) const { buffer[0] = 0; };
};
