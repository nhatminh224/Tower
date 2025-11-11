#include "GameState.h"
#include "MapSelectionState.h"
#include "Utility.h"
#include <iostream>
#include <sstream>

GameState::GameState(StateStack& stack, Context context)
    : State(stack, context),
    TOWER_RANGE(300.f),
    currentLevelIndex(MapSelectionState::levelID),
    waveIndex(0),
    levels(),
    isGameOver(false),
    isGameWin(false),
    hasPressedPlay(false),
    showNotEnough(false),
    showInfo(false),
    showTowerRange(false),
    isChoosingTower(false)
{
    // Load music
    auto& musicFlag = *getContext().isMusicOn;
    auto& musicState = *getContext().currentMusic;
    auto& musicHolder = *getContext().musics;

    // Stop menu music if it was playing
    if (musicState == MusicState::Menu)
    {
        musicHolder.get(Musics::MusicMenu).stop();
    }
    if (musicFlag) {
        auto& gameMusic = musicHolder.get(Musics::MusicGame);
        gameMusic.setLoop(true);
        gameMusic.play();
        musicState = MusicState::Game;
    }

    // Load textures
    backgroundTexture[0] = &getContext().textures->get(Textures::Map1);
    backgroundTexture[1] = &getContext().textures->get(Textures::Map2);
    backgroundTexture[2] = &getContext().textures->get(Textures::Map3);
    backgroundTexture[3] = &getContext().textures->get(Textures::Map4);

    mainTowerTexture = &getContext().textures->get(Textures::MainTower);

    // Load Tower textures
    towerTexture[0] = &getContext().textures->get(Textures::Tower1);
    towerTexture[1] = &getContext().textures->get(Textures::Tower2);
    towerTexture[2] = &getContext().textures->get(Textures::Tower3);
    towerTexture[3] = &getContext().textures->get(Textures::Tower1plus);
    towerTexture[4] = &getContext().textures->get(Textures::Tower2plus);
    towerTexture[5] = &getContext().textures->get(Textures::Tower3plus);

    // Load Tower Infos
    infoTexture[0] = &getContext().textures->get(Textures::Info1);
    infoTexture[1] = &getContext().textures->get(Textures::Info2);
    infoTexture[2] = &getContext().textures->get(Textures::Info3);
    infoTexture[3] = &getContext().textures->get(Textures::Info1plus);
    infoTexture[4] = &getContext().textures->get(Textures::Info2plus);
    infoTexture[5] = &getContext().textures->get(Textures::Info3plus);

    // Load buttons's texture for choosing tower
    towerChoosingButtons[0].setTexture(context.textures->get(Textures::tower1Button));
    towerChoosingButtons[1].setTexture(context.textures->get(Textures::tower2Button));
    towerChoosingButtons[2].setTexture(context.textures->get(Textures::tower3Button));
    towerChoosingCircle.setTexture(context.textures->get(Textures::circle));

    // Load Bullet textures
    bulletTexture[0] = &getContext().textures->get(Textures::Bomb);
    bulletTexture[1] = &getContext().textures->get(Textures::Fire);
    bulletTexture[2] = &getContext().textures->get(Textures::Ice);
    bulletTexture[3] = &getContext().textures->get(Textures::Bomb);
    bulletTexture[4] = &getContext().textures->get(Textures::Fire);
    bulletTexture[5] = &getContext().textures->get(Textures::Ice);
    font = getContext().fonts->get(Fonts::BruceForever);

    // Load info tower texture
    for (int i = 0; i < 6; ++i)
        infoSprite[i].setTexture(*infoTexture[i]);

    // Load Functional Button
    upgradeButton[0].setTexture(context.textures->get(Textures::upgradeButton75));
    upgradeButton[1].setTexture(context.textures->get(Textures::upgradeButton100));
    upgradeButton[2].setTexture(context.textures->get(Textures::upgradeButton125));
    sellButton.setTexture(context.textures->get(Textures::sellButton));

    // Load Construction Icon
    for (int i = 0; i < 7; ++i) {
        constructionicons[i].setTexture(context.textures->get(Textures::constructionicon));
        // set bool
        towerconstructed[i] = false;
    }

    // Load UI texture 
    pauseButton.setTexture(context.textures->get(Textures::pauseButton));
    pauseButton.setPosition(1880.f, 50.f);
    centerOrigin(pauseButton);

    commingWave.setTexture(context.textures->get(Textures::commingWave));
    if (currentLevelIndex == 3)
        commingWave.setPosition(60.f, 700.f);
    else
        commingWave.setPosition(60.f, 780.f);
    centerOrigin(commingWave);

    heartIcon.setTexture(context.textures->get(Textures::heartIcon));
    heartIcon.setPosition(140.f, 60.f);
    centerOrigin(heartIcon);

    currencyIcon.setTexture(context.textures->get(Textures::currencyIcon));
    currencyIcon.setPosition(380.f, 60.f);
    centerOrigin(currencyIcon);

    waveIcon.setTexture(context.textures->get(Textures::waveIcon));
    waveIcon.setPosition(140.f, 130.f);
    centerOrigin(waveIcon);

    circleRange.setFillColor(Color(0, 0, 0, 100));
    circleRange.setRadius(TOWER_RANGE);
    circleRange.setOrigin(TOWER_RANGE, TOWER_RANGE);

    notEnoughText.setFont(font);
    notEnoughText.setCharacterSize(36);
    notEnoughText.setString("Not enough money!");
    notEnoughText.setFillColor(Color::Green);
    notEnoughText.setOutlineColor(Color::Black);
    notEnoughText.setOutlineThickness(2.f);
    notEnoughText.setStyle(Text::Bold);
    notEnoughText.setPosition(1920.f * 0.5f - 180.f, 1080.f * 0.5f - 20.f);

    // Initialize 4 levels (levelID, enemyCount, waveCount, towerMaxCount, startGold) 
    clevel level1(1, 45, 3, 5, 200);
    level1.setWaves({ {FAST_SCOUT, 20}, {HEAVY_WALKER, 15}, {RANGED_MECH, 20} }); // 10, 15, 20
    level1.loadMap(mainTowerTexture, backgroundTexture[0], 1);

    clevel level2(2, 65, 3, 6, 400);
    level2.setWaves({ {FAST_SCOUT, 15}, {HEAVY_WALKER, 20}, {RANGED_MECH, 30} }); // 15, 20, 30
    level2.loadMap(mainTowerTexture, backgroundTexture[1], 2);

    clevel level3(3, 100, 4, 6, 700);
    level3.setWaves({ {FAST_SCOUT, 20}, {HEAVY_WALKER, 25}, {RANGED_MECH, 30}, {HEAVY_WALKER, 25} }); // 20, 25, 30, 25
    level3.loadMap(mainTowerTexture, backgroundTexture[2], 3);

    clevel level4(4, 175, 5, 7, 1000);
    level4.setWaves({ {FAST_SCOUT, 25}, {HEAVY_WALKER, 30}, {RANGED_MECH, 35}, {FAST_SCOUT, 40}, {HEAVY_WALKER, 45} }); // 25, 30, 35, 40, 45
    level4.loadMap(mainTowerTexture, backgroundTexture[3], 4);

    levels.push_back(level1);
    levels.push_back(level2);
    levels.push_back(level3);
    levels.push_back(level4);

    // Setup enemy data
    enemyData[RANGED_MECH] = {
        &getContext().textures->get(Textures::Enemy1_Walk),
        &getContext().textures->get(Textures::Enemy1_Attack),
        &getContext().textures->get(Textures::Enemy1_Death),
        6, 6, 6, // frames walk, attack, death
        0.1f, 0.1f, 0.1f, // animation speeds
        126, 123, // frame width, frame height of walk sprite sheet
        125, 125, // frame width, frame height of attack sprite sheet
        125, 125, // frame width, frame height of death sprite sheet
        1.f, 1.f, // set scale
    };

    enemyData[FAST_SCOUT] = {
        &getContext().textures->get(Textures::Enemy2_Fly),
        &getContext().textures->get(Textures::Enemy2_Attack),
        &getContext().textures->get(Textures::Enemy2_Death),
        4, 6, 6,
        0.1f, 0.1f, 0.1f,
        209, 203,
        209, 203,
        209, 203,
        0.5f, 0.5f,
    };

    enemyData[HEAVY_WALKER] = {
        &getContext().textures->get(Textures::Enemy3_Walk),
        &getContext().textures->get(Textures::Enemy3_Attack),
        &getContext().textures->get(Textures::Enemy3_Death),
        6, 6, 6,
        0.1f, 0.1f, 0.1f,
        213, 211,
        212, 210,
        212, 210,
        0.5f, 0.5f,
    };

    // Load Sound 
    if (*getContext().isSoundOn)
    {
        bulletBombSound.setBuffer(getContext().soundBuffers->get(SoundBuffers::BulletBomb));
        bulletFireSound.setBuffer(getContext().soundBuffers->get(SoundBuffers::BulletFire));
        bulletLaserSound.setBuffer(getContext().soundBuffers->get(SoundBuffers::BulletLaser));

        enemyAttackSound.setBuffer(getContext().soundBuffers->get(SoundBuffers::EnemyAttack));
        enemyDeadSound.setBuffer(getContext().soundBuffers->get(SoundBuffers::EnemyDead));
        enemyWalkSound.setBuffer(getContext().soundBuffers->get(SoundBuffers::EnemyWalk));

        towerPlaceSound.setBuffer(getContext().soundBuffers->get(SoundBuffers::TowerPlace));
        TowerUpgradeSound.setBuffer(getContext().soundBuffers->get(SoundBuffers::TowerUpgrade));
    }

    loadLevel(currentLevelIndex);
}

void GameState::draw()
{
    RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
    window.clear(Color::Black);

    window.draw(backgroundSprite);

    // Draw Construction Icons
    int numbericons;

    if (currentLevelIndex == 0) numbericons = 5;
    else if (currentLevelIndex == 3) numbericons = 7;
    else numbericons = 6;

    for (int i = 0; i < numbericons; ++i)
        if (towerconstructed[i] == false)
            window.draw(constructionicons[i]);

    curMap->drawPowerStations(window);
    window.draw(curMap->getMainTower().getSprite());
    curMap->getMainTower().drawHealthBar(window);

    window.draw(heartIcon);
    window.draw(currencyIcon);
    window.draw(waveIcon);

    window.draw(hp);
    window.draw(gold);
    window.draw(wave);

    if (!hasPressedPlay && !isGameOver && !isGameWin) {
        if (commingWave.getGlobalBounds().contains(mousePos))
            commingWave.setScale(0.5f, 0.5f);
        else
            commingWave.setScale(0.4f, 0.4f);
        window.draw(commingWave);
    }

    if (pauseButton.getGlobalBounds().contains(mousePos))
        pauseButton.setScale(1.1f, 1.1f);
    else
        pauseButton.setScale(1.f, 1.f);
    window.draw(pauseButton);

    if (showTowerRange)
        window.draw(circleRange);

    for (const auto& e : enemies) {
        if (!e.hasReachedEnd() || e.getState() == DEATH) {

            // Draw enemy
            window.draw(e.getSprite());

            // Draw enemy's hp bar
            float spriteHeight = e.getSprite().getGlobalBounds().height;
            float barWidth = 50.f;
            float barHeight = 6.f;
            float barX = e.getX() - barWidth + 15.f;
            float barY = e.getY() - spriteHeight / 2.f - 45.f;

            // Black outline
            RectangleShape hpOutline(Vector2f(barWidth, barHeight));
            hpOutline.setPosition(barX, barY);
            hpOutline.setFillColor(Color::Black);
            window.draw(hpOutline);

            // Green hp bar, decrease gradually
            float hpPercent = static_cast<float>(e.getHealth()) / cenemy::getHealthByType(e.getType());
            hpPercent = max(0.f, min(1.f, hpPercent));

            RectangleShape hpBar(Vector2f(barWidth * hpPercent, barHeight));
            hpBar.setPosition(barX, barY);
            hpBar.setFillColor(Color::Green);
            window.draw(hpBar);
        }
    }

    for (const auto& tower : towers)
        window.draw(tower.getSprite());

    for (const auto& tower : towers)
        if (tower.isEffectPlaying())
            window.draw(tower.getEffectSprite());

    for (auto& b : bullets) {
        if (b.isActive())
            window.draw(b.getSprite());
        else if (b.isCollisionPlaying())
            window.draw(b.getCollisionSprite());
    }


    // Draw choosingTowerButton
    if (isChoosingTower) {
        window.draw(towerChoosingCircle);

        for (int i = 0; i < 3; i++) {
            if (towerChoosingButtons[i].getGlobalBounds().contains(mousePos))
                towerChoosingButtons[i].setScale(1.1f, 1.1f);
            else
                towerChoosingButtons[i].setScale(1.f, 1.f);
            window.draw(towerChoosingButtons[i]);
        }
    }

    // Draw Tower Infos
    if (showInfo && selectedinfo >= 3) {
        window.draw(infoSprite[selectedinfo - 3]);

        if (sellButton.getGlobalBounds().contains(mousePos))
            sellButton.setScale(1.1f, 1.1f);
        else
            sellButton.setScale(1.f, 1.f);
        window.draw(sellButton);

        if (selectedinfo - 3 < 3) {
            if (upgradeButton[selectedinfo - 3].getGlobalBounds().contains(mousePos))
                upgradeButton[selectedinfo - 3].setScale(1.1f, 1.1f);
            else
                upgradeButton[selectedinfo - 3].setScale(1.f, 1.f);
            window.draw(upgradeButton[selectedinfo - 3]);
        }
    }

    // Draw text "Not enough money!" 
    if (showNotEnough)
        window.draw(notEnoughText);
}

bool GameState::handleEvent(const Event& event)
{
    RenderWindow& window = *getContext().window;

    if (event.type == Event::Closed)
        window.close();

    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        // Add pause button
        Vector2f mousePos = getContext().window->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));

        // Click commingWave icon to spawn enemies
        if (commingWave.getGlobalBounds().contains(mousePos) && !hasPressedPlay) {
            hasPressedPlay = true;
            spawnEnemies();
            return false;
        }

        // Request pushing pause state when click pause button
        if (pauseButton.getGlobalBounds().contains(mousePos)) {
            requestStackPush(States::Pause);
            return false;
        }

        // Check if mouse is over UI elements (avoid placing tower in these cases)
        if (commingWave.getGlobalBounds().contains(mousePos) || pauseButton.getGlobalBounds().contains(mousePos))
            return false;

        pair<int, int> td;
        int mx = event.mouseButton.x;
        int my = event.mouseButton.y;

        // If player is currently choosing a tower to place on a tile
        if (isChoosingTower) {
            Sprite* clickedButton = nullptr;
            int towerType = -1;

            if (towerChoosingButtons[0].getGlobalBounds().contains(mx, my)) {
                clickedButton = &towerChoosingButtons[0];
                towerType = 0;
            }
            else if (towerChoosingButtons[1].getGlobalBounds().contains(mx, my)) {
                clickedButton = &towerChoosingButtons[1];
                towerType = 1;
            }
            else if (towerChoosingButtons[2].getGlobalBounds().contains(mx, my)) {
                clickedButton = &towerChoosingButtons[2];
                towerType = 2;
            }

            if (clickedButton && towerType != -1)
            {
                if (player.spendMoney(GameConstants::TOWER_COSTS[towerType])) {
                    // Play sound when place tower
                    if (*getContext().isSoundOn == true)
                        towerPlaceSound.play();

                    if (towers.size() < levels[currentLevelIndex].getTowerMaxCount()) {
                        ctower t;
                        td = MapHandle::getTowerdes(currentLevelIndex, selectedTile.getRow(), selectedTile.getCol());
                        int itower = MapHandle::findBlockmap(currentLevelIndex, td.first, td.second);
                        t.init(*towerTexture[towerType],
                            curMap->getMap()[td.first][td.second].getPixelX(),
                            curMap->getMap()[td.first][td.second].getPixelY(), currentLevelIndex, itower);
                        t.setLocation(cpoint(td.first, td.second, 1));
                        t.setMapForBullet(curMap->getMap());
                        t.getBullet().setSpeed(8);
                        t.setType(towerType);

                        switch (towerType) {
                        case 0: t.initEffect(getContext().textures->get(Textures::BombShootEffect), 30, 23, 5, 0.05f); break;
                        case 1: t.initEffect(getContext().textures->get(Textures::FireShootEffect), 30, 23, 5, 0.05f); break;
                        case 2: t.initEffect(getContext().textures->get(Textures::IceShootEffect), 30, 23, 5, 0.05f); break;
                        }

                        int index = MapHandle::findBlockmap(currentLevelIndex, td.first, td.second);
                        towerconstructed[index] = true;

                        towers.push_back(t);
                        MapHandle::setCmap(currentLevelIndex, *curMap, selectedTile.getRow(), selectedTile.getCol(), towerType + 3);

                        // Save when new tower placed
                        int tCurLevel = currentLevelIndex;
                        SaveManagement::playerResult[tCurLevel].status = -1; // not finished
                        SaveManagement::playerResult[tCurLevel].stars = 0;
                        SaveManagement::playerResult[tCurLevel].curWave = levels[tCurLevel].getCurrentWaveIndex();
                        SaveManagement::playerResult[tCurLevel].curGold = (player.getMoney() == 0) ? -1 : player.getMoney();
                        SaveManagement::playerResult[tCurLevel].towers.clear();
                        for (int i = 0; i < towers.size(); i++)
                        {
                            ctower tempTower;
                            tempTower.setType(towers[i].getType());
                            tempTower.setLocation(towers[i].getLocation());
                            SaveManagement::playerResult[tCurLevel].towers.push_back(tempTower);
                        }
                        SaveManagement::save(SaveManagement::playerName);
                    }
                }

                else {
                    // Turn on toast
                    showNotEnough = true;
                    notEnoughClock.restart();

                    auto mousePos = getContext().window->mapPixelToCoords(
                        Vector2i(event.mouseButton.x, event.mouseButton.y));

                    isChoosingTower = false;
                    return false;
                }
            }

            isChoosingTower = false;
            return false;

        }

        // Handle tower upgrade button click
        if (showInfo && upgradeButton[selectedinfo - 3].getGlobalBounds().contains(mx, my)) {
            int tileC = selectedinfo;
            if (tileC >= 3 && tileC <= 5) {
                if (player.spendMoney(GameConstants::UPGRADE_COSTS[tileC - 3])) {
                    // Play sound when upgrade tower
                    if (*getContext().isSoundOn == true)
                        TowerUpgradeSound.play();

                    // Get the designated tile and upgrade C value
                    int row = -1, col = -1;
                    td = MapHandle::getTowerdes(currentLevelIndex, selectedRow, selectedCol);
                    row = td.first;
                    col = td.second;
                    if (row != -1 && col != -1) {
                        int newC = tileC + 3;
                        // Update map C value for upgraded tower
                        MapHandle::setCmap(currentLevelIndex, *curMap, row, col, newC);
                        // Update the tower's type and texture
                        for (auto& t : towers) {
                            if (t.getLocation().getRow() == row && t.getLocation().getCol() == col) {
                                t.setType(newC - 3);
                                int itower = MapHandle::findBlockmap(currentLevelIndex, row, col);
                                t.init(*towerTexture[newC - 3],
                                    curMap->getMap()[row][col].getPixelX(),
                                    curMap->getMap()[row][col].getPixelY(), currentLevelIndex, itower);

                                // Save when a tower upgraded
                                int tCurLevel = currentLevelIndex;
                                SaveManagement::playerResult[tCurLevel].status = -1; // not finished
                                SaveManagement::playerResult[tCurLevel].stars = 0;
                                SaveManagement::playerResult[tCurLevel].curWave = levels[tCurLevel].getCurrentWaveIndex();
                                SaveManagement::playerResult[tCurLevel].curGold = (isGameOver || isGameWin) ? levels[currentLevelIndex].getStartGold() : player.getMoney();
                                SaveManagement::playerResult[tCurLevel].towers.clear();

                                for (int i = 0; i < towers.size(); i++)
                                {
                                    ctower tempTower;
                                    tempTower.setType(towers[i].getType());
                                    tempTower.setLocation(towers[i].getLocation());
                                    SaveManagement::playerResult[tCurLevel].towers.push_back(tempTower);
                                }

                                SaveManagement::save(SaveManagement::playerName);

                                break;
                            }
                        }
                    }

                    showInfo = false;
                    showTowerRange = false;
                    selectedinfo = -1;
                    return false;
                }

                else {
                    // Turn on toast
                    showNotEnough = true;
                    notEnoughClock.restart();

                    auto mousePos = getContext().window->mapPixelToCoords(
                        Vector2i(event.mouseButton.x, event.mouseButton.y));

                    return false;
                }
            }
        }

        // Handle tower sell button click
        if (showInfo && sellButton.getGlobalBounds().contains(mx, my)) {
            int tileC = selectedinfo;
            int row = -1, col = -1;

            td = MapHandle::getTowerdes(currentLevelIndex, selectedRow, selectedCol);
            row = td.first;
            col = td.second;

            if (row != -1 && col != -1) {
                for (auto t = towers.begin(); t != towers.end(); ++t) {
                    // Update map C value for upgraded tower
                    MapHandle::setCmap(currentLevelIndex, *curMap, row, col, 2);
                    // Sell tower
                    if (t->getLocation().getRow() == row && t->getLocation().getCol() == col) {
                        towers.erase(t);

                        if (tileC >= 3 && tileC <= 5)
                            player.addMoney(GameConstants::TOWER_COSTS[tileC - 3] / 10 * 7);
                        else
                            player.addMoney((GameConstants::UPGRADE_COSTS[tileC - 6] + GameConstants::TOWER_COSTS[tileC - 6]) / 10 * 7);

                        int index = MapHandle::findBlockmap(currentLevelIndex, row, col);
                        towerconstructed[index] = false;

                        // Save when a tower upgraded
                        int tCurLevel = currentLevelIndex;
                        SaveManagement::playerResult[tCurLevel].status = -1; // not finished
                        SaveManagement::playerResult[tCurLevel].stars = 0;
                        SaveManagement::playerResult[tCurLevel].health = (isGameWin || isGameOver) ? curMap->getMainTower().getMaxHealth() : curMap->getMainTower().getHealth();
                        SaveManagement::playerResult[tCurLevel].curWave = levels[tCurLevel].getCurrentWaveIndex();
                        SaveManagement::playerResult[tCurLevel].curGold = (isGameOver || isGameWin) ? levels[currentLevelIndex].getStartGold() : player.getMoney();
                        SaveManagement::playerResult[tCurLevel].towers.clear();

                        for (int i = 0; i < towers.size(); i++)
                        {
                            ctower tempTower;
                            tempTower.setType(towers[i].getType());
                            tempTower.setLocation(towers[i].getLocation());
                            SaveManagement::playerResult[tCurLevel].towers.push_back(tempTower);
                        }

                        SaveManagement::save(SaveManagement::playerName);

                        break;
                    }
                }

                showInfo = false;
                showTowerRange = false;
                selectedinfo = -1;
                return false;
            }
        }
        // Handle click on map tiles
        cpoint clicked = cpoint::fromXYToRowCol(mx, my);
        if (clicked.getRow() >= 0 && clicked.getRow() < cpoint::MAP_ROW &&
            clicked.getCol() >= 0 && clicked.getCol() < cpoint::MAP_COL) {

            int c = curMap->getMap()[clicked.getRow()][clicked.getCol()].getC();

            // If clicked on a tile with C = 2 (candidate tower tile)
            if (c == 2 && towers.size() < levels[currentLevelIndex].getTowerMaxCount()) {
                selectedTile = clicked;
                isChoosingTower = true;

                // Display tower options at click location
                auto buttons = MapHandle::getTowerButtons(currentLevelIndex, selectedTile.getRow(), selectedTile.getCol());
                if (buttons.size() == 4) {
                    for (int i = 0; i < 3; ++i)
                        towerChoosingButtons[i].setPosition(buttons[i]);
                    towerChoosingCircle.setPosition(buttons[3]);
                }

                return false;
            }

            // Clicked Sprite
            ctower* clickedTower = nullptr;
            for (auto& t : towers) {
                if (t.getSprite().getGlobalBounds().contains(mx, my)) {
                    clickedTower = &t;
                    break;
                }
            }

            // If clicked on a tile that already has a tower (C >= 3)
            if (c >= 3 || clickedTower) {
                selectedinfo = c;
                if (clickedTower->getSprite().getTexture() == towerTexture[0]) selectedinfo = 3;
                else if (clickedTower->getSprite().getTexture() == towerTexture[1]) selectedinfo = 4;
                else if (clickedTower->getSprite().getTexture() == towerTexture[2]) selectedinfo = 5;
                else if (clickedTower->getSprite().getTexture() == towerTexture[3]) selectedinfo = 6;
                else if (clickedTower->getSprite().getTexture() == towerTexture[4]) selectedinfo = 7;
                else if (clickedTower->getSprite().getTexture() == towerTexture[5]) selectedinfo = 8;
                selectedRow = clicked.getRow();
                selectedCol = clicked.getCol();
                // Get fixed display tile for showing info panel
                pair<int, int> fixed = MapHandle::getTowerdes(currentLevelIndex, clicked.getRow(), clicked.getCol());
                if (fixed.first == -1 && fixed.second == -1) {
                    selectedRow = clicked.getRow() + 3;
                    selectedCol = clicked.getCol();
                    fixed = MapHandle::getTowerdes(currentLevelIndex, selectedRow, selectedCol);
                }

                float fx = (float)fixed.second * cpoint::TILE_SIZE;
                float fy = (float)fixed.first * cpoint::TILE_SIZE;

                // Display tower info panel and upgrade button if upgradeable
                infoSprite[selectedinfo - 3].setPosition((float)fx + cpoint::TILE_SIZE, (float)fy - 4 * cpoint::TILE_SIZE);
                sellButton.setPosition((float)fx + cpoint::TILE_SIZE * 3.5f + 200.f, (float)fy - 4.85f * cpoint::TILE_SIZE + 30.f);
                circleRange.setPosition((float)fx + cpoint::TILE_SIZE * 0.5f, (float)fy + cpoint::TILE_SIZE * 0.5f - 80.f);

                if (selectedinfo >= 3 && selectedinfo <= 5)
                    upgradeButton[selectedinfo - 3].setPosition((float)fx + cpoint::TILE_SIZE + 300.f, (float)fy - 4.85f * cpoint::TILE_SIZE + 110.f);


                showInfo = true;
                showTowerRange = true;
                return false;
            }
        }

        // Hide info panel if clicked outside
        showInfo = false;
        showTowerRange = false;
        selectedinfo = -1;

        return true;
    }
}

bool GameState::update(Time dt)
{
    // Set Icons
    MapHandle::setIconsmap(currentLevelIndex, constructionicons);

    //Enemy update - modify tower damage logic
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        cenemy& e = *it;
        bool shouldErase = false;

        cenemy& e0 = *enemies.begin();
        // Handle dead enemies first
        if (e.isDead()) {

            if (e.getState() == DEATH) {
                // Only give reward if not already given
                if (!e.hasGivenReward()) {
                    player.addMoney(e.getResources());
                    e.markRewardGiven();
                }
            }

            if (e.getState() == DEATH && e.hasFinishedDeathAnim())
                shouldErase = true;

            // Dead but still playing death animation - just update
            else {
                e.updateAnimation(dt.asSeconds());
                ++it;
                shouldErase = true;
            }
            continue;
        }

        // Handle living enemies
        if (!e.hasReachedEnd()) {
            if (e.getCurrentTarget() < e.getPathLength()) {
                // Normal path following
                float targetX = e.getP()[e.getCurrentTarget()].getPixelX();
                float targetY = e.getP()[e.getCurrentTarget()].getPixelY();

                float dx = targetX - e.getX();
                float dy = targetY - e.getY();
                float len = sqrt(dx * dx + dy * dy);

                if (len < 1.f) {
                    e.incrementTarget();

                    // Check if reached final target
                    if (e.getCurrentTarget() >= e.getPathLength()) {
                        // If not already attacking, trigger attack
                        if (e.getState() != ATTACK)
                            e.triggerAttack();

                        if (*getContext().isSoundOn) {
                            bulletLaserSound.setVolume(20);
                            bulletLaserSound.play();
                        }

                        // Check if attack animation has finished
                        if (e.hasFinishedAttackAnim()) {
                            e.reachEnd();
                            shouldErase = true;
                        }

                        curMap->getMainTower().takeDamage(e.getDamage());

                        if (curMap->getMainTower().isDestroyed()) {
                            isGameOver = true;
                            isGameWin = false;
                        }
                    }
                }
                else {
                    // Normal movement
                    dx /= len;
                    dy /= len;

                    // Rotate enemy sprite (only in level 3)
                    if (currentLevelIndex == 2) {
                        if (dx < -0.1f)
                            e.faceLeft(e.getType());
                        else if (dx > 0.1f || dy < -0.1f)
                            e.faceRight(e.getType());
                    }

                    e.move(dx * e.getSpeedByType(e.getType()) * dt.asSeconds(),
                        dy * e.getSpeedByType(e.getType()) * dt.asSeconds());
                }
            }

            // Update animation for living enemies
            e.updateAnimation(dt.asSeconds());
        }

        // Erase enemy if marked, otherwise move to next
        if (shouldErase)
            it = enemies.erase(it);
        else
            ++it;
    }

    // Clean up dead enemies and enemies that reached end
    enemies.erase(
        remove_if(enemies.begin(), enemies.end(),
            [](const cenemy& e) {
                return (e.isDead() && e.hasFinishedDeathAnim())
                    || e.hasFinishedAttackAnim()
                    || e.hasReachedEnd();
            }),
        enemies.end()
    );


    // The wave must be the last wave
    if (!isGameOver && !isGameWin && curMap->getMainTower().getHealth() > 0 && enemies.empty() && hasPressedPlay) {
        clevel& level = levels[currentLevelIndex];

        if (!level.isLastWave()) {
            level.nextWave();

            // Save every new wave
            int tCurLevel = currentLevelIndex;
            SaveManagement::playerResult[tCurLevel].status = -1; // not finished
            SaveManagement::playerResult[tCurLevel].stars = 0;
            SaveManagement::playerResult[tCurLevel].health = (isGameWin || isGameOver) ? curMap->getMainTower().getMaxHealth() : curMap->getMainTower().getHealth();
            SaveManagement::playerResult[tCurLevel].curWave = levels[tCurLevel].getCurrentWaveIndex();
            SaveManagement::playerResult[tCurLevel].curGold = (isGameOver || isGameWin) ? levels[currentLevelIndex].getStartGold() : player.getMoney();
            SaveManagement::playerResult[tCurLevel].towers.clear();
            for (int i = 0; i < towers.size(); i++)
            {
                ctower tempTower;
                tempTower.setType(towers[i].getType());
                tempTower.setLocation(towers[i].getLocation());
                SaveManagement::playerResult[tCurLevel].towers.push_back(tempTower);
            }
            SaveManagement::save(SaveManagement::playerName);

            spawnEnemies();
        }

        else
            isGameWin = true;
    }

    if (isGameOver) {
        int tCurLevel = currentLevelIndex;
        if (!SaveManagement::playerResult[currentLevelIndex].win || tCurLevel == 0)
        {
            SaveManagement::playerResult[currentLevelIndex].status = 0; // locked
            SaveManagement::playerResult[tCurLevel].stars = 0;
            SaveManagement::playerResult[tCurLevel].health = curMap->getMainTower().getMaxHealth();
            SaveManagement::playerResult[tCurLevel].curWave = 0;
            SaveManagement::playerResult[tCurLevel].curGold = levels[currentLevelIndex].getStartGold();
            SaveManagement::playerResult[tCurLevel].towers.clear();
            SaveManagement::save(SaveManagement::playerName);
        }

        requestStackPush(States::Defeat);
        isGameOver = false;
    }

    // Push VictoryState
    if (isGameWin)
    {
        // Save results
        int tCurLevel = currentLevelIndex;
        SaveManagement::playerResult[tCurLevel].win = true;
        SaveManagement::playerResult[tCurLevel].status = 1; // game done
        SaveManagement::playerResult[tCurLevel].stars = calStars();
        SaveManagement::playerResult[tCurLevel].health = curMap->getMainTower().getMaxHealth();
        SaveManagement::playerResult[tCurLevel].curWave = 0;
        SaveManagement::playerResult[tCurLevel].curGold = levels[currentLevelIndex].getStartGold();
        SaveManagement::playerResult[tCurLevel].towers.clear();
        SaveManagement::save(SaveManagement::playerName);

        *getContext().victoryStars = calStars();
        requestStackPush(States::Victory);
        isGameWin = false;
    }

    // Tower attack logic: target enemies and shoot
    for (auto& tower : towers) {
        tower.addShootTimer(dt.asSeconds());

        // Trigger shootEffect
        if (tower.getTargetEnemyIdx() != -1 && tower.getShootTimer() > 0.8f && !tower.isEffectPlaying())
            tower.startEffect();

        bool validTarget = false;
        int idx = tower.getTargetEnemyIdx();

        // Check current target still valid
        if (idx >= 0 && idx < enemies.size() && !enemies[idx].hasReachedEnd() && !enemies[idx].isDead()) {
            float dist = hypot(tower.getSprite().getPosition().x - enemies[idx].getX(),
                tower.getSprite().getPosition().y - enemies[idx].getY());
            if (dist <= TOWER_RANGE)
                validTarget = true;
        }

        // Find new target if needed
        if (!validTarget) {
            tower.setTargetEnemyIdx(-1);
            for (int i = 0; i < enemies.size(); i++) {
                if (enemies[i].hasReachedEnd() || enemies[i].isDead()) continue;
                float dist = hypot(tower.getSprite().getPosition().x - enemies[i].getX(),
                    tower.getSprite().getPosition().y - enemies[i].getY());
                if (dist <= TOWER_RANGE) {
                    tower.setTargetEnemyIdx(i);
                    break;
                }
            }
        }

        // Shoot bullet when cooldown is over
        if (tower.getTargetEnemyIdx() != -1 && tower.getShootTimer() > 1.f) {
            tower.startEffect();
            tower.resetShootTimer();

            cbullet b;
            cbullet b1;
            int t = tower.getType();

            const Texture* tex = nullptr;
            int frameW = 0, frameH = 0, totalFrames = 0;
            float animSpeed = 0.f, scale = 1.f;

            switch (t) {
            case 0: // Tower type 1 - Bomb
                tex = bulletTexture[0];
                frameW = 16; frameH = 15; totalFrames = 7;
                animSpeed = 0.05f; scale = 4.f;

                b.initCollisionEffect(getContext().textures->get(Textures::BombImpact), (float)542 / 9, 62, 9, 0.05f, 1.7f);

                break;

            case 1: // Tower type 2 - Fire
                tex = bulletTexture[1];
                frameW = 1667; frameH = 1167; totalFrames = 4;
                animSpeed = 0.05f; scale = 0.1f;

                b.initCollisionEffect(getContext().textures->get(Textures::FireImpact), (float)283 / 5, 44, 5, 0.09f, 1.7f);

                break;

            case 2: // Tower type 3 - Ice
                tex = bulletTexture[2];
                frameW = 141; frameH = 114; totalFrames = 5;
                animSpeed = 0.05f; scale = 0.8f;

                b.initCollisionEffect(getContext().textures->get(Textures::IceImpact), (float)388 / 6, 69, 6, 0.09f, 1.4f);

                break;

            case 3: // Tower type 1 - Upgraded - Bomb
                tex = bulletTexture[0];
                frameW = 16; frameH = 15; totalFrames = 7;
                animSpeed = 0.05f; scale = 4.f;

                b.initCollisionEffect(getContext().textures->get(Textures::BombImpact), (float)542 / 9, 62, 9, 0.05f, 1.7f);
                b1.initCollisionEffect(getContext().textures->get(Textures::BombImpact), (float)542 / 9, 62, 9, 0.05f, 1.7f);

                break;

            case 4: // Tower type 2 - Upgraded - Fire
                tex = bulletTexture[1];
                frameW = 1667; frameH = 1167; totalFrames = 4;
                animSpeed = 0.05f; scale = 0.1f;

                b.initCollisionEffect(getContext().textures->get(Textures::FireImpact), (float)283 / 5, 44, 5, 0.09f, 1.7f);
                b1.initCollisionEffect(getContext().textures->get(Textures::FireImpact), (float)283 / 5, 44, 5, 0.09f, 1.7f);

                break;

            case 5: // Tower type 3 - Upgraded - Ice
                tex = bulletTexture[2];
                frameW = 141; frameH = 114; totalFrames = 5;
                animSpeed = 0.05f; scale = 0.8f;

                b.initCollisionEffect(getContext().textures->get(Textures::IceImpact), (float)388 / 6, 69, 6, 0.09f, 1.4f);
                b1.initCollisionEffect(getContext().textures->get(Textures::IceImpact), (float)388 / 6, 69, 6, 0.09f, 1.4f);

                break;

            default:
                tex = bulletTexture[0];
                frameW = 16; frameH = 15; totalFrames = 7;
                animSpeed = 0.05f; scale = 4.f;
                break;
            }

            b.init(*tex,
                tower.getSprite().getPosition().x, tower.getSprite().getPosition().y - 40.f,
                frameW, frameH, totalFrames, animSpeed, scale);
            b1.init(*tex,
                tower.getSprite().getPosition().x, tower.getSprite().getPosition().y - 40.f,
                frameW, frameH, totalFrames, animSpeed, scale);

            b.setTargetIdx(tower.getTargetEnemyIdx());
            b1.setTargetIdx(tower.getTargetEnemyIdx());

            // Set speed for bullet by tower type
            if (tower.getType() == 1) b.setSpeed(4);
            else if (tower.getType() == 2) b.setSpeed(3);
            else if (tower.getType() == 3) {
                b.setSpeed(6);
                b1.setSpeed(4.5);
            }
            else if (tower.getType() == 4) {
                b.setSpeed(5);
                b1.setSpeed(3.5);
            }
            else if (tower.getType() == 5) {
                b.setSpeed(4);
                b1.setSpeed(2.5);
            }
            else b.setSpeed(5);

            // Set dmg for bullet by tower type
            if (tower.getType() == 1) b.setDamage(2);
            else if (tower.getType() == 2) b.setDamage(3);
            else if (tower.getType() == 3) {
                b.setDamage(4);
                b1.setDamage(0);
            }
            else if (tower.getType() == 4) {
                b.setDamage(5);
                b1.setDamage(0);
            }
            else if (tower.getType() == 5) {
                b.setDamage(6);
                b1.setDamage(0);
            }
            else b.setDamage(1);

            bullets.push_back(b);
            if (tower.getType() == 3 || tower.getType() == 4 || tower.getType() == 5) bullets.push_back(b1);
        }

        tower.updateEffect(dt.asSeconds());
    }

    // Bullet logic: track and hit enemies
    for (auto& b : bullets) {
        if (b.isActive()) {
            int idx = b.getTargetIdx();

            if (idx < 0 || idx >= enemies.size() || enemies[idx].hasReachedEnd() || enemies[idx].isDead())
                b.deactivate();

            else if (b.checkCollision(enemies[idx])) {
                if (*getContext().isSoundOn) {
                    bulletBombSound.setVolume(25);
                    bulletBombSound.play();
                }

                b.triggerCollision(enemies[idx].getX(), enemies[idx].getY());
                enemies[idx].takeDamage(b.getDamage());
            }

            else {
                b.trackEnemy(enemies[idx], dt.asSeconds());
                b.updateAnimation(dt.asSeconds());
            }
        }

        b.updateCollision(dt.asSeconds());
    }

    // Update powerStations animation
    curMap->updatePowerStation(dt.asSeconds());

    // Remove inactive bullets
    bullets.erase(remove_if(bullets.begin(), bullets.end(),
        [](cbullet& b) { return b.isRemovable(); }), bullets.end());

    // Update mainTower hp & gold
    hp.setString(to_string(curMap->getMainTower().getHealth()));
    gold.setString(to_string(player.getMoney()));
    wave.setString(to_string(levels[currentLevelIndex].getCurrentWaveIndex() + 1) + "/" + to_string(levels[currentLevelIndex].getWaveCount()));

    // Turn off toast after 1s
    if (showNotEnough && notEnoughClock.getElapsedTime().asSeconds() > NOT_ENOUGH_DURATION)
        showNotEnough = false;

    return true;
}

void GameState::loadLevel(int index) {
    RenderWindow& window = *getContext().window;

    if (index < 0 || index >= levels.size()) return;

    currentLevelIndex = index;

    // Load map & pathfinding
    curMap = &levels[currentLevelIndex].getMap();
    cenemy& ce = curMap->getEnemy();
    ce.findPath(curMap->getMap(), ce.getStart(), ce.getEnd());

    // Load map data & texture & mainTowerMaxHealth for the current level

    levels[currentLevelIndex].loadMap(mainTowerTexture, backgroundTexture[index], currentLevelIndex + 1);
    backgroundSprite.setTexture(*backgroundTexture[currentLevelIndex]);
    window.setSize(backgroundTexture[currentLevelIndex]->getSize());

    // Load powerStation effect for this level
    const Texture& powerStationTex = getContext().textures->get(Textures::powerStation);
    string powerStationFiles = "Media/Text files/powerStation_map" + to_string(currentLevelIndex + 1) + ".txt";
    curMap->loadPowerStationsFromFile(powerStationTex, powerStationFiles, 92, 92, 0.03f);

    // Apply the position to the tower sprite
    curMap->getMainTower().getPosition();

    // Reset enemy, tower, bullet...
    enemies.clear();
    towers.clear();
    bullets.clear();

    // Reset game flags and wave index
    isGameOver = false;
    isGameWin = false;
    hasPressedPlay = false; // Reset play state for new level

    // SaveManagement: Load wave, health, gold
    int loadWave = SaveManagement::playerResult[currentLevelIndex].curWave;
    int loadHealth = SaveManagement::playerResult[currentLevelIndex].health;
    int loadGold = SaveManagement::playerResult[currentLevelIndex].curGold;

    levels[currentLevelIndex].setCurrentWaveIndex(loadWave);            // wave

    if (loadHealth != 0)                                                // health
        curMap->getMainTower().setCurrentHealth(loadHealth);
    curMap->getMainTower().takeDamage(0);
    curMap->getMainTower().drawHealthBar(window);

    if (loadGold == 0)                                                  // gold
        player.setMoney(levels[currentLevelIndex].getStartGold());
    else if (loadGold == -1)
        player.setMoney(0);
    else
        player.setMoney(loadGold);

    // SaveManagement: Load towers' properties
    int savedTowers = SaveManagement::playerResult[currentLevelIndex].towers.size();
    for (int i = 0; i < savedTowers; i++)
    {
        ctower tTower;
        int tType = SaveManagement::playerResult[currentLevelIndex].towers[i].getType();
        cpoint tLoc = SaveManagement::playerResult[currentLevelIndex].towers[i].getLocation();

        tTower.setType(tType);
        switch (tType) {
        case 0: tTower.initEffect(getContext().textures->get(Textures::BombShootEffect), 30, 23, 5, 0.05f); break;
        case 1: tTower.initEffect(getContext().textures->get(Textures::FireShootEffect), 30, 23, 5, 0.05f); break;
        case 2: tTower.initEffect(getContext().textures->get(Textures::IceShootEffect), 30, 23, 5, 0.05f); break;
        }
        tTower.setLocation(tLoc);
        int itower = MapHandle::findBlockmap(currentLevelIndex, tLoc.getRow(), tLoc.getCol());
        tTower.init(*towerTexture[tType],
            tLoc.getPixelX(),
            tLoc.getPixelY(), currentLevelIndex, itower);
        tTower.setMapForBullet(curMap->getMap());
        tTower.getBullet().setSpeed(8);
        int index = MapHandle::findBlockmap(currentLevelIndex, tLoc.getRow(), tLoc.getCol());
        towerconstructed[index] = true;

        towers.push_back(tTower);

        MapHandle::setCmap(currentLevelIndex, *curMap, tLoc.getRow(), tLoc.getCol(), tType + 3);
    }

    // Set up text to display main tower hp (demo)
    hp.setFont(font);
    hp.setCharacterSize(30);
    hp.setFillColor(Color::White);
    hp.setPosition(160.f, 60.f);
    hp.setString(to_string(curMap->getMainTower().getHealth()));
    centerOrigin(hp);

    // Set up text to display gold (demo)
    gold.setFont(font);
    gold.setCharacterSize(30);
    gold.setFillColor(Color::White);
    gold.setPosition(410.f, 60.f);
    gold.setString(to_string(player.getMoney()));
    centerOrigin(gold);

    // Set up text to display current wave index
    wave.setFont(font);
    wave.setCharacterSize(30);
    wave.setString(to_string(levels[currentLevelIndex].getCurrentWaveIndex() + 1) + "/" + to_string(levels[currentLevelIndex].getWaveCount()));
    wave.setFillColor(Color::White);
    wave.setPosition(160.f, 130.f);
    centerOrigin(wave);

    MapHandle::initTowerButtonData();
}

void GameState::spawnEnemies() {
    // Get the current level and its wave information
    const auto& waves = levels[currentLevelIndex].getWaves();
    if (waveIndex >= waves.size()) return;

    clevel& level = levels[currentLevelIndex];

    pair<EnemyType, int> info = level.getCurrentWaveInfo();
    EnemyType type = info.first;
    int count = info.second;
    cenemy& ce = curMap->getEnemy();

    for (int i = 0; i < count; i++) {
        cenemy enemy;
        enemy.loadFromData(enemyData[type]);

        enemy.setStart(ce.getStart());
        enemy.setEnd(ce.getEnd());
        enemy.findPath(curMap->getMap(), ce.getStart(), ce.getEnd());

        // Offset enemies so they don't overlap
        cpoint startPoint = ce.getP()[0];
        float pixelX = startPoint.getPixelX() - i * 120.f;
        float pixelY = startPoint.getPixelY();

        enemy.init(type, pixelX, pixelY, enemy.getHealthByType(type), enemyData[type]);
        enemy.setCurr(startPoint);
        enemies.push_back(enemy);
    }

    waveIndex++; // Update wave
}

int GameState::calStars() {
    int curHealth = curMap->getMainTower().getHealth();
    int maxHealth = curMap->getMainTower().getMaxHealth();
    float healthRatio = static_cast<float>(curHealth) / maxHealth;

    // If gameWin()
    // If currentMainTowerHealth >= 80% mainTowerMaxHealth -> 3 stars
    if (healthRatio >= 0.8f)
        return 3;
    // If currentMainTowerHealth >= 50% mainTowerMaxHealth -> 2 stars
    else if (healthRatio >= 0.5f)
        return 2;
    // If currentMainTowerHealth > 1 -> 1 star
    else if (curHealth > 0)
        return 1;
    // GameOver = true
    else
        return 0;
}

GameState::~GameState()
{
    auto& musicFlag = *getContext().isMusicOn;
    auto& musicState = *getContext().currentMusic;
    auto& musicHolder = *getContext().musics;

    // Stop game music if it was playing
    if (musicState == MusicState::Game) {
        musicHolder.get(Musics::MusicGame).stop();

        // Resume menu music if it was playing before
        if (musicFlag) {
            auto& menuMusic = musicHolder.get(Musics::MusicMenu);
            menuMusic.setLoop(true);
            menuMusic.play();
            musicState = MusicState::Menu;
        }
        else {
            musicState = MusicState::None;
        }
    }
}