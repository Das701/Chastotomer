#pragma once


class MenuPage;


namespace Menu
{
    void Init();

    void Draw();

    void Update();

    String ChannelSettings();
    
    MenuPage* OpenedPage();

    void SetOpenedPage(MenuPage *page);
};
