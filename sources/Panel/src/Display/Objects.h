#pragma once


class Object
{
public:

    virtual ~Object() {};

    void Update()  { if (needUpdate) { Draw(); needUpdate = false; } }
    
    // Установить флаг необходимости перерисовки
    void Refresh() { needUpdate = true; }

protected:

    virtual void Draw() = 0;

private:

    bool needUpdate;
};


class Indicator : public Object
{
public:

protected:

    virtual void Draw();

private:
};
