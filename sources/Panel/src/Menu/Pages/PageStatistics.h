#pragma once
#include "Utils/Stack.h"


class Page;


class PageStatistics
{
public:

    static Page *self;

    static void Clear();
    static void Draw();
};
