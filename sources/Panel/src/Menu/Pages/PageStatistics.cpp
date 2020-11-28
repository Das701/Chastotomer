#include "defines.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Utils/String.h"


using namespace Primitives;


DEF_BUTTON(bClear, "Очистить", "Очистить график", PageStatistics::Clear);


static void OnPress_Exit()
{
    Menu::SetOpenedPage(Channel::A.pageModes);
}

DEF_BUTTON(bExit, "Выход", "Переход на страницу выбра режима", OnPress_Exit);


static Item *items[7] =
{
    &bClear,
    &bExit,
    nullptr
};


static Page pageShowStatistics(items, nullptr);

Page *PageStatistics::self = &pageShowStatistics;


void PageStatistics::Clear()
{
    MathFPGA::Comparator::values.Clear();
}


void PageStatistics::Draw()
{
    int x0 = 10;
    int y0 = 10;
    int width = Display::PHYSICAL_WIDTH - 20;
    int height = 91;

    Rectangle(width, height).Fill(x0, y0, Color::GRAY_50);

    if (MathFPGA::Comparator::values.Size() < 2)
    {
        return;
    }

    int startElement = MathFPGA::Comparator::values.Size() - 6;

    if (startElement < 0)
    {
        startElement = 0;
    }

    for (int i = startElement; i < MathFPGA::Comparator::values.Size(); i++)
    {
        Text(String("%d - %f", i, MathFPGA::Comparator::values[i])).Write(x0 + 330, y0 + 1 + 15 * (i - startElement), Color::WHITE);
    }

    double min = 1e100;
    double max = 0;

    for (int i = 0; i < MathFPGA::Comparator::values.Size(); i++)
    {
        if (MathFPGA::Comparator::values[i] < min) { min = MathFPGA::Comparator::values[i]; }

        if (MathFPGA::Comparator::values[i] > max) { max = MathFPGA::Comparator::values[i]; }
    }

    float stepX = (float)width / (float)(MathFPGA::Comparator::values.Size() - 1);
    float stepY = (float)height / (float)(max - min);

    y0 = y0 + height;

    int x = x0;
    int y = y0 - (int)(stepY * (MathFPGA::Comparator::values[0] - min));
    
    Color::BLACK.SetAsCurrent();

    Point().Draw(x, y);

    for (int i = 1; i < MathFPGA::Comparator::values.Size(); i++)
    {
        int x1 = x0 + (int)((float)(i - 1) * stepX);
        int x2 = x0 + (int)((float)i * stepX);

        int y1 = y0 - (int)(stepY * (MathFPGA::Comparator::values[i - 1] - min));
        int y2 = y0 - (int)(stepY * (MathFPGA::Comparator::values[i] - min));

        Line().Draw(x1, y1, x2, y2);
    }

    String strMAX("%f", max);
    String strMIN("%f", min);

    Rectangle(Font::GetLengthText(strMAX.c_str()) + 2, 16).Fill(10, 10, Color::GRAY_50);
    Rectangle(Font::GetLengthText(strMIN.c_str()) + 2, 16).Fill(10, height - 6);

    Text(strMAX).Write(11, 11, Color::BLACK);
    Text(strMIN).Write(11, height - 5);
}
