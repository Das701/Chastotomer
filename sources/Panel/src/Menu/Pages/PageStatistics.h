#pragma once


class Page;
struct ValuePICO;


class PageStatistics
{
public:

    static Page *self;

    static void AppendValue(ValuePICO &value);
    static void Clear();
    static void Draw();
};
