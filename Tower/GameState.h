#pragma once
#include "State.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "Utility.h"
#include "ctower.h"
#include "cbullet.h"
#include "cenemy.h"
#include "clevel.h"
#include "cmap.h"
#include "cBaseTower.h"
#include "MapHandle.h"
#include "Player.h"
#include "SaveManagement.h"
#include <vector>
#include <map>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>

using namespace std;
using namespace sf;

class GameState : public State
{
public:
    GameState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(Time dt);
    virtual bool handleEvent(const Event& event);

    ~GameState();

private:
    Event event;

    void loadLevel(int index);
    void spawnEnemies();
    int calStars();

    Sprite backgroundSprite;

    Font font;
    Text hp, gold, wave;

    vector<cenemy> enemies;
    vector<ctower> towers;
    vector<cbullet> bullets;
    vector<clevel> levels;

    cmap* curMap;
    int currentLevelIndex;
    int waveIndex;
    float TOWER_RANGE;

    bool isGameOver;
    bool isGameWin;
    bool hasPressedPlay = false;

    map<EnemyType, EnemyAnimationData> enemyData;
    Texture* backgroundTexture[4];
    Texture* towerTexture[6];
    Texture* bulletTexture[6];
    Texture* mainTowerTexture;

    // UI
    Sprite pauseButton;
    Sprite commingWave;
    Sprite upgradeButton[3];
    Sprite sellButton;
    Sprite towerChoosingButtons[3];
    Sprite towerChoosingCircle;
    Sprite heartIcon;
    Sprite waveIcon;
    Sprite currencyIcon;
    CircleShape circleRange;

    // Construction Icons
    Sprite constructionicons[7];
    bool towerconstructed[7];

    // Tower Infos
    Texture* infoTexture[6];
    Sprite infoSprite[6];
    bool showInfo = false;
    bool showTowerRange = false;
    int selectedinfo;

    // Icon appear when click
    bool isChoosingTower = false;
    cpoint selectedTile;  // clicked pixel
    // Player for money
    Player player;

    // Upgrade Assistance
    int selectedRow;
    int selectedCol;

    // Sound
    Sound bulletBombSound;
    Sound bulletFireSound;
    Sound bulletLaserSound;

    Sound enemyAttackSound;
    Sound enemyDeadSound;
    Sound enemyWalkSound;

    Sound towerPlaceSound;
    Sound TowerUpgradeSound;

    // Toast "Not enough money"
    Text notEnoughText;
    bool showNotEnough = false;
    Clock notEnoughClock;
    const float NOT_ENOUGH_DURATION = 1.2f; // sec
};