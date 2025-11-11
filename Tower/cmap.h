#pragma once
#include "cenemy.h"
#include "ctower.h"
#include "cBaseTower.h"
#include "FrameAnimator.h"
#include <vector>

using namespace std;

struct PowerStation {
    sf::Sprite sprite;
    FrameAnimator animator;
};

class cmap
{
private:
    cenemy _ce;
    ctower _ctw;
    cpoint _m[cpoint::MAP_ROW][cpoint::MAP_COL];

    // Base Tower properties
    cBaseTower _mainTower;
    cpoint _mainTowerTile; // Tile position of main tower
    sf::Vector2f _mainTowerPixelPos;  // Actual pixel position

    vector <cpoint> _towerBuiltPos;
    vector <bool> _isBuilt;
    sf::Sprite _background;
    std::vector<PowerStation> _powerStations;

public:
    cmap();

    void resetMapData();
    void makeMapData(sf::Texture* mainTowerTexture, sf::Texture* mapTexture, int levelID);
    void addPowerStation(const sf::Texture& tex, sf::Vector2f pos, int frameW, int frameH, float speed);
    void loadPowerStationsFromFile(const sf::Texture& tex, const std::string& path, int frameW, int frameH, float speed);
    void updatePowerStation(float dt);
    void drawPowerStations(sf::RenderWindow& window);

    // Getter
    cenemy& getEnemy() { return _ce; }
    ctower& getTower() { return _ctw; }
    cpoint(&getMap())[cpoint::MAP_ROW][cpoint::MAP_COL]{ return _m; }
    sf::Sprite& getBackground() { return _background; }
    cpoint getMainTowerTile() const { return _mainTowerTile; }
    cBaseTower& getMainTower() { return _mainTower; }
    sf::Vector2f getMainTowerPosition() const { return _mainTowerPixelPos; }
    bool isMainTowerDestroyed() const { return _mainTower.isDestroyed(); }

    // Setter
    void setMainTowerTile(const cpoint& tilePos);
};
