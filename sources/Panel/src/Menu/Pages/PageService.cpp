#include "defines.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageService.h"


extern Item *items[7];


static void OnPress_Information()
{

}

DEF_BUTTON(bInformation, "Инфо", OnPress_Information);

static Item *items[7] =
{
    &bInformation,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};


static Page pageService(items);

Page *PageService::self = &pageService;
