#pragma once


class Object
{
public:

    Object(int x, int y, int width, int height) : x0(x), y0(y), width0(width), height0(height), needUpdate(true) {}
    virtual ~Object() {};

    void Update();
   
    // Установить флаг необходимости перерисовки
    void Refresh() { needUpdate = true; if (Display::DrawingToBuffer()) { Display::Refresh(); } }

protected:

    virtual void DrawToBuffer() = 0;
    virtual bool DrawToHardware() = 0;

    const int x0;
    const int y0;
    const int width0;
    const int height0;

private:

    void FillBackground();

    bool needUpdate;
};


class DataZone : public Object
{
public:
    DataZone() : Object(0, 150, Display::PHYSICAL_WIDTH, 50) {}

protected:

    virtual void DrawToBuffer();
    virtual bool DrawToHardware();
};
