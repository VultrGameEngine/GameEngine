#pragma once

struct CellComponent
{
    bool state = false;
    int row = 0;
    int col = 0;
    int zol = 0;

    static CellComponent Create(int row = 0, int col = 0, int zol = 0)
    {
        CellComponent component = CellComponent();
        component.state = false;
        component.row = row;
        component.col = col;
        component.zol = zol;
        return component;
    }

    CellComponent Toggle()
    {
        CellComponent component =
            CellComponent::Create(this->row, this->col, this->zol);
        component.state = !this->state;
        return component;
    }
};
