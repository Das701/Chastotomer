#pragma once


class Page;


namespace Menu
{
    void Draw();

    bool Update();

    char *ChannelSettings();
    
    Page* OpenedPage();

    void SetOpenedPage(Page *page);
};
