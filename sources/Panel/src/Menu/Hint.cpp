#include "defines.h"
#include "Hint.h"
#include "Menu/MenuItems.h"
#include <stm32f4xx_hal.h>
#include <cstring>


/// ������ ����� ��������� ��� ���������� ��������������� �����
static char hint[100];
/// ����, ��� �������� ��������� ���������
static const Item *item = nullptr;
/// ����� ��������� ����������� ���������
static uint timeHideHint;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Hint::Create(const Item *i)
{
    item = i;
    item->CreateHint(hint);
    timeHideHint = HAL_GetTick() + 5000;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *Hint::Text()
{
    return HAL_GetTick() <  timeHideHint ? hint : "";
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const Item *Hint::FromItem()
{
    return item;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Hint::Hide()
{
    hint[0] = 0;
    item = nullptr;
    timeHideHint = HAL_GetTick() - 1;
}
