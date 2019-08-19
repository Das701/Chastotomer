#include "defines.h"
#include "PageModes.h"


static Page pModes;

Page *PageModes::self = &pModes;

static Control *controls[] = { nullptr };


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawClosed(int x, int y)
{
//    Rectangle(5, 5).Fill(x + 1, y + 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageModes::Init()
{
    pModes.Init(controls, DrawClosed);
}
