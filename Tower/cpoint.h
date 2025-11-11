#pragma once
#include <cmath>

class cpoint
{
public:
    static const int MAP_ROW = 27;
    static const int MAP_COL = 48;
    static const int TILE_SIZE = 40;

private:
    int row, col, c;

public:
    cpoint();
    cpoint(int trow, int tcol, int tc);

    int getRow() const;
    int getCol() const;
    int getC() const;
    int getPixelX() const;
    int getPixelY() const;
    void setC(int tc);

    static cpoint fromXYToRowCol(int x, int y, int c = 0);
    static cpoint fromRowColToXY(int row, int col, int c = 0);
};
