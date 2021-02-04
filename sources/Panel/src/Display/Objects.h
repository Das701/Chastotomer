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

    Object(int x, int y, int width, int height) : x0(0), y0(0), modeDraw(ModeDraw::ToBuffer), left(x), top(y), width(width), height(height), needUpdate(false) {}
    virtual ~Object() {};

    void Update(ModeDraw::E mode);
   
    // Установить флаг необходимости перерисовки
    void Refresh();

protected:

    virtual bool Draw() = 0;

    int x0;
    int y0;
    ModeDraw::E modeDraw;

    int Width() { return width; }

private:

    void FillBackground();

    const int left;
    const int top;
    const int width;
    const int height;
    bool needUpdate;
};


class DataZone : public Object
{
public:
    DataZone() : Object(10, 140, Display::PHYSICAL_WIDTH - 50, 60) {}

protected:

    virtual bool Draw();
};


class ProgressBarTimeMeasureZone : public Object
{
public:
    ProgressBarTimeMeasureZone() : Object(273, 90, 55, 5) {};

    static void Reset();

protected:

    virtual bool Draw();

    static uint timeStart;      // Здесь хранится время последнего чтения. Используется для расчёта отрисовки
};


class SynchroZone : public Object
{
public:

    SynchroZone() : Object(390, 120, 20, 20) {}

    static void Fire();

protected:

    virtual bool Draw();

    int CalculateSize();

    static uint timeStart;

    static const int MAX_SIZE = 15;
};
