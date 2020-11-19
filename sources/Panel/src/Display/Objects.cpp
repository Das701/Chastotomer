#include "defines.h"
#include "Display/Display.h"
#include "Display/Objects.h"
#include "Display/Text.h"
#include "Display/Font/Font.h"
#include "Hardware/MathFPGA.h"
#include "Utils/String.h"
#include <cctype>
#include <cstdlib>


void Object::Update()
{
    if (needUpdate)
    {
        Display::Prepare(width0, height0);

        FillBackground();

        if (Draw())
        {
            needUpdate = false;
        }

        Display::Restore();
    }
}


void Object::FillBackground()
{

}


bool DataZone::Draw()
{
    // ��������� ����������� � ���������.
    // ������ ������ �������������� � ������ ����������� ������


    uint8 buffer[100][100];

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            buffer[i][j] = (uint8)(std::rand() % 16);
        }
    }


    HAL_FSMC::SendBuffer(&buffer[0][0], 10, 50, 100, 100);

    return true;
}


bool DataZone::Draw1()
{
    if (Display::InDrawingPart(y0, height0))
    {
        String data = MathFPGA::Data::GiveDigits();

        if (data[0] != 0)
        {
            if (std::isdigit(data[0]) != 0 || data[0] == ' ' || data[0] == '-')         // ������, ���� ������
            {
                FontBig::Write(data.c_str(), x0 + 10, y0, Color::WHITE);
            }
            else
            {
                int x = x0;

                if (data[0] == '�')         { x += 40;  }   // ������������
                else if (data[0] == '=')    { x += 150; }   // ������� �� ����

                Font::Set(TypeFont::GOSTB28B);
                Text(data.c_str()).Write(x, y0 + 15, Color::WHITE);
                Font::Set(TypeFont::GOSTAU16BOLD);
            }
        }

        FontMid::Write(MathFPGA::Data::GiveUnits().c_str(), 360, 170, Color::WHITE);

        return true;
    }

    return false;
}
