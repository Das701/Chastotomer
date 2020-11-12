#pragma once
#include "Utils/String.h"


class Item;


class Hint
{
public:

    // Создаёт подсказку для item
    static void Create(const Item *item);

    // Возвращает текст подсказки
    static String Text();

    // Возвращает итем, для которого действует подсказка
    static const Item *UnderItem();

    // Спрятать подсказку
    static void Hide();
};
