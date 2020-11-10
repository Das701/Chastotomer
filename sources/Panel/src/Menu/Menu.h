#pragma once


class Page;


namespace Menu
{
    void Init();

    void Draw();

    bool Update();

    char *ChannelSettings();
    
    Page* OpenedPage();

    void SetOpenedPage(Page *page);
};
