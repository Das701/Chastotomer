#pragma once


class Object
{
public:

    struct ModeDraw
    {
        enum E
        {
            ToBuffer,       // Отрисовка стандартным способом - через буфер
            ToHardware      // засылка сразу в хардварный дисплей
        };
    };

    Object(int x, int y, int width, int height) : x0(x), y0(y), width0(width), height0(height), needUpdate(true) {}
    virtual ~Object() {};

    void Update();
   
    // Установить флаг необходимости перерисовки
    void Refresh() { needUpdate = true; }

protected:

    virtual bool Draw() = 0;

    const int x0;
    const int y0;
    const int width0;
    const int height0;

private:

    void FillBackground();

    bool needUpdate;

    static ModeDraw::E modeDraw;
};


class DataZone : public Object
{
public:
    DataZone() : Object(0, 150, Display::PHYSICAL_WIDTH, 50) {}

protected:

    virtual bool Draw();

    bool Draw1();

private:
};
