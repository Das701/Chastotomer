#pragma once


class Object
{
public:

    virtual ~Object() {};

    void Update(int x, int y)  { if (needUpdate) { Draw(x, y); needUpdate = false; } }
    
    // Установить флаг необходимости перерисовки
    void Refresh() { needUpdate = true; }

protected:

    virtual void Draw(int x, int y) = 0;

private:

    bool needUpdate;
};


class Indicator : public Object
{
public:

protected:

    virtual void Draw(int x, int y);

private:
};
