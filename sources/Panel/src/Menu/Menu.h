#pragma once


class Page;


namespace Menu
{
    void Init();

    void Draw();

    void Update();

    Page* OpenedPage();

    void SetOpenedPage(Page *page);
};
