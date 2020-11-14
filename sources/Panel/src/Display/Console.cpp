#include "defines.h"
#include "Display/Console.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/String.h"
#include <cstring>
#include <cstdio>


#define S_DBG_NUM_STRINGS_IN_CONSOLE 10


char Console::buffer[10][100];
bool Console::inProcessDraw = false;
bool Console::inProcessAddingString = false;
int Console::stringInConsole = 0;
int16 Console::prevMaxStrinsInConsole = -1;


void Console::Init()
{
    inProcessDraw = false;
    inProcessAddingString = false;
    stringInConsole = 0;
    prevMaxStrinsInConsole = -1;
}


void Console::Draw()
{
    if (prevMaxStrinsInConsole == -1)
    {
        prevMaxStrinsInConsole = S_DBG_NUM_STRINGS_IN_CONSOLE;
    }

    if (!IsShown() || stringInConsole == 0)
    {
        return;
    }

    inProcessDraw = true;

    int y = -1;

    for (int i = 0; i < stringInConsole; i++)
    {
        Text(String(buffer[i]).c_str()).Write(1, y, Color::BLACK);
        y += 6;
    }

    inProcessDraw = false;
}


void Console::DeleteFirstString()
{
    for (int16 i = 1; i < stringInConsole; i++)
    {
        std::strcpy(buffer[i - 1], buffer[i]); //-V2513
    }
    stringInConsole--;
}


void Console::AddString(const char *string)
{
    // \todo Мы пропускаем некоторые строки. Сделать отложенное добавление

    //if (!IsBusy())      // Страхуемся на предмет того, что сейчас не происходит вывод консоли в другом потоке
    {
        inProcessAddingString = true;

        static int count = 0;
        if (stringInConsole == S_DBG_NUM_STRINGS_IN_CONSOLE)
        {
            DeleteFirstString();
        }
        std::sprintf(buffer[stringInConsole], "%d %s", count++, string);
        stringInConsole++;

        inProcessAddingString = false;
    }
}


bool Console::IsShown()
{
    return true;
}


void Console::OnChanged_MaxStringsInConsole()
{
    // \todo Здесь, видимо, не совсем корректное поведение в случае, когда реальных строк меньше, чем максимально допустимое их количество

    int delta = prevMaxStrinsInConsole - S_DBG_NUM_STRINGS_IN_CONSOLE;

    for (int i = 0; i < delta; i++)
    {
        DeleteFirstString();
    }

    prevMaxStrinsInConsole = S_DBG_NUM_STRINGS_IN_CONSOLE;


}


bool Console::IsBusy()
{
    return inProcessDraw || inProcessAddingString;
}
