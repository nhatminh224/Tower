#pragma once
#include "cpoint.h"
#include "cmap.h"
#include <vector>
#include <string>

using namespace std;

class clevel
{
private:
	int _levelID;
	int _currentLevel;
	int _enemyCount;
	int _waveCount;
	int _towerMaxCount;
	int _startGold;
	cmap _map;
	vector<pair<EnemyType, int>> _waves;
	int _currentWaveIndex = 0;

	cBaseTower mainTower;

public:
	clevel(int levelID, int enemyCount, int waveCount, int towerMaxCount, int startGold);

	void loadMap(sf::Texture* mainTowerTexture, sf::Texture* mapTexture, int levelId);
	void nextWave();

	// Getters
	int getLevelID() const { return _levelID; }
	int getCurrentLevel() const { return _currentLevel; }
	int getEnemyCount() const { return _enemyCount; }
	int getWaveCount() const { return _waveCount; }
	int getTowerMaxCount() const { return _towerMaxCount; }
	int getStartGold() const { return _startGold; }
	cmap& getMap() { return _map; };
	const vector<std::pair<EnemyType, int>>& getWaves() const { return _waves; }
	pair<EnemyType, int> getCurrentWaveInfo() const;
	int getCurrentWaveIndex() const { return _currentWaveIndex; }
	bool isLastWave() const { return _currentWaveIndex >= _waves.size() - 1; }

	// Setters
	void setLevelID(int level) { _levelID = level; }
	void setCurrentLevel(int curLevel) { _currentLevel = curLevel; }
	void setEnemyCount(int ec) { _enemyCount = ec; }
	void setWaveCount(int wc) { _waveCount = wc; }
	void setTowerMaxCount(int towerMaxCount) { _towerMaxCount = towerMaxCount; }
	void setStartGold(int startGold) { _startGold = startGold; }
	void setMap(cmap map) { _map = map; }
	void setWaves(const vector<pair<EnemyType, int>>& waves) { _waves = waves; }
	void resetWave() { _currentWaveIndex = 0; }
	void setCurrentWaveIndex(int curWave) { _currentWaveIndex = curWave; }
};

