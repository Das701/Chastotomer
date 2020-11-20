#pragma once


class Object
{
public:

    struct ModeDraw
    {
        enum E
        {
            ToBuffer,       // ��������� ����������� �������� - ����� �����
            ToHardware      // ������� ����� � ���������� �������
        };
    };

    Object(int x, int y, int width, int height) : x0(x), y0(y), width0(width), height0(height), needUpdate(false) {}
    virtual ~Object() {};

    void Update(ModeDraw::E mode);
   
    // ���������� ���� ������������� �����������
    void Refresh() { needUpdate = true; }

protected:

    virtual bool Draw() = 0;

    int X0() { return (modeDraw == ModeDraw::ToHardware) ? 0 : x0; }
    int Y0() { return (modeDraw == ModeDraw::ToHardware) ? 0 : y0; }

private:

    void FillBackground();

    const int x0;
    const int y0;
    const int width0;
    const int height0;
    bool needUpdate;
    ModeDraw::E modeDraw;
};


class DataZone : public Object
{
public:
    DataZone() : Object(10, 151, Display::PHYSICAL_WIDTH - 50, 50) {}

protected:

    virtual bool Draw();
};
