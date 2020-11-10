#pragma once


class Observer
{
public:
    virtual ~Observer() {}
    virtual void OnEvent() = 0;
};
