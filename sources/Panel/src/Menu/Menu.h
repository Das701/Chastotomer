#pragma once


class Page;


namespace Menu
{
    void Init();

    void Draw();

    void Update();

    String ChannelSettings();
    
    Page* OpenedPage();

    void SetOpenedPage(Page *page);
};
