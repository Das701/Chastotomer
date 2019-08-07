#pragma once


class Page;


namespace Menu
{
    void Init();

    void Update();

    void Draw(int x, int y);

    Page *CurrentPage();
}
