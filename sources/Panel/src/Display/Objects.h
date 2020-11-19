#pragma once


class Object
{
public:
    virtual void Draw(int x, int y) = 0;
    virtual ~Object() {};
private:
};


class Indicator : public Object
{
public:
    virtual void Draw(int x, int y);
private:
};
