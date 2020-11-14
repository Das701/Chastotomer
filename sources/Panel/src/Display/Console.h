#pragma once


struct Console
{
    static void Init();

    static void Draw();

    static void AddString(const char *string);
    // Возвращает true, если консоль должна отображаться на экране
    static bool IsShown();
    // Эту фнукцию надо вызвыть, когда пользователь изменяет максимальное количество строк в консоли
    static void OnChanged_MaxStringsInConsole();

private:
    static char buffer[10][100];
    // true означает, что идёт процесс вывода консоли и добавлять в неё новые строки нельзя (это происходит, когда добавление идёт из прерывания)
    static bool inProcessDraw;
    // true, если происходит процесс добавления строки
    static bool inProcessAddingString;
    // Количество заполненных строк в консоли
    static int stringInConsole;
    // Здесь сохраняется предыдущее значение максимального количества строк в консоли
    static int16 prevMaxStrinsInConsole;

    static void DeleteFirstString();
    // Возвращает true, если консоль занята и с ней нельзя производить работу
    static bool IsBusy();
};

