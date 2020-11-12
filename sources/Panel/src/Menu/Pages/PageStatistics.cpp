#include "defines.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Utils/String.h"


using namespace Display::Primitives;
using namespace Display;


DEF_BUTTON(bClear, "Очистить", PageStatistics::Clear);


static void OnPress_Exit()
{
    Menu::SetOpenedPage(PageModesA::self);
}

DEF_BUTTON(bExit, "Выход", OnPress_Exit);


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
    FPGA::Comparator::values.Clear();
}


void PageStatistics::Draw()
{
    int x0 = 10;
    int y0 = 10;
    int width = Display::WIDTH - 20;
    int height = 110;

    Rectangle(width, height).Fill(x0, y0, Color::GRAY_50);

    if (FPGA::Comparator::values.Size() < 2)
    {
        return;
    }

    int startElement = FPGA::Comparator::values.Size() - 7;

    if (startElement < 0)
    {
        startElement = 0;
    }

    for (int i = startElement; i < FPGA::Comparator::values.Size(); i++)
    {
        Text(String("%d - %f", i, FPGA::Comparator::values[i]).c_str()).Write(x0 + 1, y0 + 15 * (i - startElement), Color::WHITE);
    }

    double min = 1e100;
    double max = 0;

    for (int i = 0; i < FPGA::Comparator::values.Size(); i++)
    {
        if (FPGA::Comparator::values[i] < min) { min = FPGA::Comparator::values[i]; }

        if (FPGA::Comparator::values[i] > max) { max = FPGA::Comparator::values[i]; }
    }

    float stepX = (float)width / (float)(FPGA::Comparator::values.Size() - 1);
    float stepY = (float)height / (float)(max - min);

    y0 = y0 + height;

    int x = x0;
    int y = y0 - (int)(stepY * (FPGA::Comparator::values[0] - min));
    
    Color::BLACK.SetAsCurrent();

    Point().Draw(x, y);

    for (int i = 1; i < FPGA::Comparator::values.Size(); i++)
    {
        int x1 = x0 + (int)((float)(i - 1) * stepX);
        int x2 = x0 + (int)((float)i * stepX);

        int y1 = y0 - (int)(stepY * (FPGA::Comparator::values[i - 1] - min));
        int y2 = y0 - (int)(stepY * (FPGA::Comparator::values[i] - min));

        Line().Draw(x1, y1, x2, y2);
    }

    Text(String("%f", max).c_str()).Write(0, 0);
    Text(String("%f", min).c_str()).Write(0, 10 + height);
}
