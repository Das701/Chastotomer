#pragma once
#include "Utils/Stack.h"


class MenuPage;


class PageStatistics
{
public:

    static MenuPage *self;

    static void Clear();
    static void Draw();
};
