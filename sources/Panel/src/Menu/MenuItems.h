#pragma once
#include "defines.h"
#include "Display/Colors.h"
#include "Keyboard/Keyboard.h"
#include "Utils/Observer.h"


struct TypeMeasure;
class Switch;


struct Enumeration
{
    uint8  value;           // ������� ��������� ������������
    char **names;
    char **ugo;
    const bool *correct;    // ���� ��������� �� ���� ������ �� ����� nullptr, �� ��� �������� ������������ ���������
                            // ������� : value �� ����� �������� ��������, ��������������� false
    explicit Enumeration(uint8 v, const bool *_correct = nullptr, int states = 0);
    operator int()         { return (int)value; }
    String ToString() const;
    char *UGO() const;
    int NumStates() const;
    // ���������� ������ �� ������� names, �������������� �������� value
    int IndexName() const;
    bool Is(uint8 v) const { return value == v; }
    Switch *sw;
    bool SetValue(uint8 v);
    bool ValidValue(uint8 v) const;
    int numStates;
};



class Item
{
    friend class Hint;
public:
    Item(char *_hint);
    virtual ~Item() {};

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
    virtual void CreateHint(String &hint) const = 0;
};



class Button : public Item
{
public:
    Button(char *_text, char *_hint, void (*funcPress)()) : Item(_hint), text(_text), funcOnPress(funcPress) {};

    virtual void Draw(int x, int y, int width, bool selected = false);
    virtual bool OnControl(const Control &control);
    
private:
    char *text;
    void (*funcOnPress)();

    virtual void CreateHint(String &hint) const;
};



class Switch : public Item
{
    friend class Page;
    friend class PageModes;
public:

    Switch(char *_text, char *_hint, char **_names, char **_ugo, Enumeration *_state, void(*_onClick)()) :
        Item(_hint), text(_text), funcOnPress(_onClick), state(_state)
    {
        state->names = _names;
        state->ugo = _ugo;
        state->sw = this;
    };
    virtual void Draw(int x, int y, int width, bool selected = false);
    virtual bool OnControl(const Control &control);

    void FuncOnPress() const { if (funcOnPress) { funcOnPress(); }; }

    uint8 Value() const { return state->value; }

    // ��� ���������� �� SCPI
    void FuncForSCPI(int i);

    bool SetValue(uint8 v);

private:
    char        *text;              // ������� �� �������������
    void       (*funcOnPress)();    // ��� ������� ���������� ����� ��������� ��������� �������������
    Enumeration *state;             // ����� ���������� � ���������� �������������
    virtual void CreateHint(String &hint) const;
    // ����������� � ��������� ���������
    void  NextChoice();
};



class Page : public Item, public Observer
{
    friend struct Channel;

public:
    Page(Item **_items, void (*_onEvent)(EventType::E)) :
        Item(""), selectedItem(0), items(_items), onEvent(_onEvent)
    {};

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

protected:

    // ���������� ���������� ������ �� ��������
    int NumItems() const;

    virtual void CreateHint(String &_hint) const { _hint.Free(); };

    // ���������� ������ �������� ���� � ������� num
    int WidthItem(int num) const;

    // ��������� �� ������ ��������� ����. ������������� ����.
    Item **items;

    void (*onEvent)(EventType::E);
};


class PageModes : public Page
{
public:
    PageModes(Item **items, void (*onEvent)(EventType::E)) : Page(items, onEvent) {}

    // ������� ������������� ��� ������� ������� �������
    TypeMeasure *GetTypeMeasure() const;
    int GetModeMeasure() const;
    bool ExistTypeMeasure(uint8 type) const;
    void ResetTypeAndModeMeasure();
    bool ExistModeMeasure(int mode) const;
    void ResetModeMeasure();
};
