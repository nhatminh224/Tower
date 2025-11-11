#pragma once

#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

#include "ResourceHolder.h"

enum class MusicState
{
    None,
    Menu,
    Game
};

namespace Textures
{
    enum ID
    {
        // Maps
        Map1,
        Map2,
        Map3,
        Map4,

        // Enemy 1
        Enemy1_Walk,
        Enemy1_Attack,
        Enemy1_Death,

        // Enemy 2
        Enemy2_Fly,
        Enemy2_Attack,
        Enemy2_Death,

        // Enemy 3
        Enemy3_Walk,
        Enemy3_Attack,
        Enemy3_Death,

        // Bullet
        Bomb,
        Fire,
        Ice,

        // Effect
        BombShootEffect,
        FireShootEffect,
        IceShootEffect,

        // Towers 
        Tower1,
        Tower2,
        Tower3,
        Tower1plus,
        Tower2plus,
        Tower3plus,
        MainTower,

        // Tower Infos
        Info1,
        Info2,
        Info3,
        Info1plus,
        Info2plus,
        Info3plus,

        MenuScreen,

        // Upgrade & Sell Button
        upgradeButton75,
        upgradeButton100,
        upgradeButton125,
        sellButton,

        // Construction Icon
        constructionicon,

        // UI for MenuState
        newGameButton,
        loadGameButton,
        exitButton,

        // UI for SettingState
        settingButton,
        optionsPanelInMenu,
        onButton,
        offButton,
        closeButton,

        // UI for InformationState
        infoPanel,
        infoButton,

        // UI for MapSelectionState
        mapSelectionBackground,
        homeButton,
        miniMap1,
        miniMap2,
        miniMap3,
        miniMap4,
        miniMap2Locked,
        miniMap3Locked,
        miniMap4Locked,
        wave,

        // UI for GameState
        pauseButton,
        pausePanel,
        restartButton,
        quitButton,
        commingWave,
        heartIcon,
        waveIcon,
        currencyIcon,

        // UI for VictoryState & DefeatState
        victory,
        defeat,
        star,
        restartButton2,
        quitButton2,

        // Load powerStation texture
        powerStation,

        // Load UI for choosing tower
        tower1Button,
        tower2Button,
        tower3Button,
        circle,

        // InputNameState 
        inputNameBackground,
        nextButton,

        // Load UI for SaveManagemner (Loading)
        loadBackground,
        nameBar,

        // Load collision effect 
        BombImpact,
        FireImpact,
        IceImpact,
    };
}

namespace Fonts
{
    enum ID
    {
        BruceForever,
        KnightWarrior,
        RobotTraffic,
    };
}

// Add a new namespace for game constants

namespace GameConstants
{
    // Tower costs
    const int TOWER_BASE_COST = 100;
    const int TOWER_COSTS[] = { 100, 150, 200 }; // Costs for basic tower types

    // Upgrade costs
    const int UPGRADE_COST = 75;
    const int UPGRADE_COSTS[] = { 75, 100, 125 }; // Different costs per tower type if needed

    // Tower sell refund percentage (50%)
    const float SELL_REFUND_RATE = 0.5f;

    // Enemy rewards
    const int ENEMY_REWARDS[] = { 20, 30, 40 }; // Rewards for killing different enemy types

    // Starting money
    const int STARTING_MONEY = 300;

    // Passive income (per wave or per time interval)
    const int PASSIVE_INCOME = 25;
    const float INCOME_INTERVAL = 10.0f; // Seconds between passive income
}

namespace SoundBuffers
{
    enum ID
    {
        // Bullet
        BulletBomb,
        BulletFire,
        BulletLaser,

        // Enemy
        EnemyAttack,
        EnemyDead,
        EnemyWalk,

        // GameEnd
        GameOver,
        GameWin,

        // Tower
        TowerPlace,
        TowerUpgrade,

        // Mouse Click
        UIclick
    };
}

namespace Musics
{
    enum ID
    {
        MusicGame,
        MusicMenu
    };
}

// Type aliases for easy access
using TextureHolder = ResourceHolder<sf::Texture, Textures::ID>;
using FontHolder = ResourceHolder<sf::Font, Fonts::ID>;
using SoundBufferHolder = ResourceHolder<sf::SoundBuffer, SoundBuffers::ID>;
using MusicHolder = ResourceHolder<sf::Music, Musics::ID>;