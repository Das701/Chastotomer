#pragma once


class Page;


namespace Menu
{
    void Init();

    void Draw();

    bool Update();

    String ChannelSettings();
    
    Page* OpenedPage();

    void SetOpenedPage(Page *page);
};
