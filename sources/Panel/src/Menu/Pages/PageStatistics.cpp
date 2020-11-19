#include "defines.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Utils/String.h"


using namespace Primitives;


DEF_BUTTON(bClear, "��������", "�������� ������", PageStatistics::Clear);


static void OnPress_Exit()
{
    Menu::SetOpenedPage(PageModesA::self);
}

DEF_BUTTON(bExit, "�����", "������� �� �������� ����� ������", OnPress_Exit);


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
    int width = Display::WIDTH - 20;
    int height = 110;

    Rectangle(width, height).Fill(x0, y0, Color::GRAY_50);

    if (MathFPGA::Comparator::values.Size() < 2)
    {
        return;
    }

    int startElement = MathFPGA::Comparator::values.Size() - 7;

    if (startElement < 0)
    {
        startElement = 0;
    }

    for (int i = startElement; i < MathFPGA::Comparator::values.Size(); i++)
    {
        Text(String("%d - %f", i, MathFPGA::Comparator::values[i])).Write(x0 + 330, y0 + 2 + 15 * (i - startElement), Color::WHITE);
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

    Text(String("%f", max)).Write(11, 11);
    Text(String("%f", min)).Write(11, height - 5);
}
