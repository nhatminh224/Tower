#pragma once
#include <iostream>
#include <cmath>
#include "cmap.h"
#include <vector>
#include <SFML/System.hpp>

using namespace std;
using namespace sf;

class MapHandle {
private:
    inline static float TILE_SIZE = 40.f;
    static vector<vector<Vector2f>> towerButtonData;

public:
    static void setCmap1(cmap& map, int a, int b, int C);
    static void setCmap2(cmap& map, int a, int b, int C);
    static void setCmap3(cmap& map, int a, int b, int C);
    static void setCmap4(cmap& map, int a, int b, int C);
    static pair <int, int> getTowerdes1(int a, int b);
    static pair <int, int> getTowerdes2(int a, int b);
    static pair <int, int> getTowerdes3(int a, int b);
    static pair <int, int> getTowerdes4(int a, int b);
    static void setIconsmap1(Sprite* icon);
    static void setIconsmap2(Sprite* icon);
    static void setIconsmap3(Sprite* icon);
    static void setIconsmap4(Sprite* icon);
    static int findBlockmap1(int a, int b);
    static int findBlockmap2(int a, int b);
    static int findBlockmap3(int a, int b);
    static int findBlockmap4(int a, int b);

    static void setCmap(int index, cmap& map, int a, int b, int C);
    static pair <int, int> getTowerdes(int index, int a, int b);
    static void setIconsmap(int index, Sprite* icon);
    static int findBlockmap(int index, int a, int b);

    static void initTowerButtonData();
    static vector<Vector2f> getTowerButtons(int levelIndex, int row, int col);
}; #pragma once
