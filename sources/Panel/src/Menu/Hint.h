#pragma once


class Item;


class Hint
{
public:
    /// Создаёт подсказку для item
    static void Create(const Item *item);
    /// Возвращает текст подсказки
    static char *Text();
    /// Возвращает итем, для которого действует подсказка
    static const Item *UnderItem();
    /// Спрятать подсказку
    static void Hide();
};
