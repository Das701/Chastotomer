#include "defines.h"
#include "PageIndication.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"


using namespace Display::Primitives;
using namespace Display;


/// ����� ������� ����������� ���������� ���������
static Button bDisplayTime("�-���");

/// ����� �������� ����������
static Button bRefGenerator("��");

/// ����� ��������� �������
static Button bLaunchSource("���");

/// ���� � ����� ���������� 
static Button bCalibration("�����");

/// ���������(����������) ������ ������
static Button bMemoryMode("���");

static Item *items[] =
{
    &bDisplayTime,
    &bRefGenerator,
    &bLaunchSource,
    &bCalibration,
    &bMemoryMode,
    nullptr
};

static Page pageIndication(items);

Page *PageIndication::self = &pageIndication;
