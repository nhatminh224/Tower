#include "clevel.h"
#include <iostream>

using namespace std;

clevel::clevel(int levelID, int enemyCount, int waveCount, int towerMaxCount, int startGold) {
    _levelID = levelID;
    _enemyCount = enemyCount;
    _waveCount = waveCount;
    _towerMaxCount = towerMaxCount;
    _startGold = startGold;
    _currentLevel = 1;
    _currentWaveIndex = 0; // Initialize wave index
}

void clevel::loadMap(sf::Texture* mainTowerTexture, sf::Texture* mapTexture, int levelId) {
    // First validate textures
    if (!mainTowerTexture || !mapTexture) {
        cerr << "Error: Null texture passed to loadMap()" << endl;
        return;
    }

    // Load map data
    _map.makeMapData(mainTowerTexture, mapTexture, levelId);

    // Set main tower texture and properties
    if (mainTowerTexture) {
        sf::Sprite& towerSprite = mainTower.getSprite();
        towerSprite.setTexture(*mainTowerTexture);

        // Set proper texture rect if using sprite sheets
        towerSprite.setTextureRect(sf::IntRect(0, 0,
            mainTowerTexture->getSize().x,
            mainTowerTexture->getSize().y));

        // Center origin if needed
        sf::FloatRect bounds = towerSprite.getLocalBounds();
        towerSprite.setOrigin(bounds.width / 2, bounds.height / 2);

        // Set proper scale (adjust as needed)
        towerSprite.setScale(1.0f, 1.0f);

        // Set position based on map data
        cpoint towerTile = _map.getMainTowerTile();
        float towerX = _map.getMap()[towerTile.getRow()][towerTile.getCol()].getPixelX();
        float towerY = _map.getMap()[towerTile.getRow()][towerTile.getCol()].getPixelY();
        mainTower.setPosition(towerX, towerY); // Adjust Y offset as needed
    }
}

void clevel::nextWave() {
    if (_currentWaveIndex + 1 < _waves.size())
        _currentWaveIndex++;
}

pair<EnemyType, int> clevel::getCurrentWaveInfo() const {
    if (_currentWaveIndex < _waves.size())
        return _waves[_currentWaveIndex];
    else
        return { HEAVY_WALKER, 0 };
}
