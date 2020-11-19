#pragma once


class Object
{
public:

    virtual ~Object() {};

    void Update();
   
    // ���������� ���� ������������� �����������
    void Refresh() { needUpdate = true; }

protected:

    virtual bool Draw() = 0;

private:

    bool needUpdate;
};


class DataZone : public Object
{
public:

protected:

    virtual bool Draw();

    bool Draw1();

private:
};
