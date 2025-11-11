#include "Application.h"
#include "StateIdentifiers.h"
#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"
#include "VictoryState.h"
#include "DefeatState.h"
#include "SettingState.h"
#include "InformationState.h"
#include "MapSelectionState.h"
#include "InputNameState.h"
#include "SaveManagement.h"

Application::Application()
    : mWindow(sf::VideoMode(1920, 1080), "Tower Defense")
    , mTextures()
    , mFonts()
    , mPlayer()
    // Initialize StateStack with the application context (window, resources, audio, settings)
    , mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer, mVictoryStars, mSoundBuffers, mMusics, isMusicOn, isSoundOn, currentMusic))
{
    mWindow.setVerticalSyncEnabled(true); // Smoother rendering

    registerStates();
    mStateStack.pushState(States::Menu); // Start with menu screen

    // Load fonts
    mFonts.load(Fonts::BruceForever, "Media/Fonts/BruceForeverRegular-X3jd2.ttf");
    mFonts.load(Fonts::KnightWarrior, "Media/Fonts/KnightWarrior-w16n8.otf");
    mFonts.load(Fonts::RobotTraffic, "Media/Fonts/RobotTrafficDemo-BLPlw.ttf");

    // Load map textures
    mTextures.load(Textures::Map1, "Media/Textures/map 1.png");
    mTextures.load(Textures::Map2, "Media/Textures/map 2.png");
    mTextures.load(Textures::Map3, "Media/Textures/map 3.png");
    mTextures.load(Textures::Map4, "Media/Textures/map 4.png");

    // Load tower textures
    mTextures.load(Textures::MainTower, "Media/Textures/mainTower.png");
    mTextures.load(Textures::Tower1, "Media/Textures/tower 1.png");
    mTextures.load(Textures::Tower2, "Media/Textures/tower 2.png");
    mTextures.load(Textures::Tower3, "Media/Textures/tower 3.png");
    mTextures.load(Textures::Tower1plus, "Media/Textures/tower 1 - upgraded.png");
    mTextures.load(Textures::Tower2plus, "Media/Textures/tower 2 - upgraded.png");
    mTextures.load(Textures::Tower3plus, "Media/Textures/tower 3 - upgraded.png");

    // Load effect textures
    mTextures.load(Textures::BombShootEffect, "Media/Textures/BombShootEffect.png");
    mTextures.load(Textures::FireShootEffect, "Media/Textures/FireShootEffect.png");
    mTextures.load(Textures::IceShootEffect, "Media/Textures/IceShootEffect.png");

    // Load tower infos
    mTextures.load(Textures::Info1, "Media/Textures/infotower1.png");
    mTextures.load(Textures::Info2, "Media/Textures/infotower2.png");
    mTextures.load(Textures::Info3, "Media/Textures/infotower3.png");
    mTextures.load(Textures::Info1plus, "Media/Textures/infotower1+.png");
    mTextures.load(Textures::Info2plus, "Media/Textures/infotower2+.png");
    mTextures.load(Textures::Info3plus, "Media/Textures/infotower3+.png");

    // Load bullet sprites
    mTextures.load(Textures::Bomb, "Media/Textures/bomb_sprite_sheet.png");
    mTextures.load(Textures::Fire, "Media/Textures/fire_bullet_sprite_sheet.png");
    mTextures.load(Textures::Ice, "Media/Textures/ice_bullet_sprite_sheet.png");

    // Load enemy animation textures
    mTextures.load(Textures::Enemy1_Walk, "Media/Textures/enemy_1_walk_126_123_6.png");
    mTextures.load(Textures::Enemy1_Attack, "Media/Textures/enemy_1_attack_125_125_6.png");
    mTextures.load(Textures::Enemy1_Death, "Media/Textures/enemy_1_death_125_125_6.png");
    mTextures.load(Textures::Enemy2_Fly, "Media/Textures/enemy_2_fly_209_203_4.png");
    mTextures.load(Textures::Enemy2_Attack, "Media/Textures/enemy_2_attack_209_203_6.png");
    mTextures.load(Textures::Enemy2_Death, "Media/Textures/enemy_2_death_209_203_6.png");
    mTextures.load(Textures::Enemy3_Walk, "Media/Textures/enemy_3_walk_213_211_6.png");
    mTextures.load(Textures::Enemy3_Attack, "Media/Textures/enemy_3_attack_212_210_6.png");
    mTextures.load(Textures::Enemy3_Death, "Media/Textures/enemy_3_death_212_210_6.png");

    // Load functional buttons
    mTextures.load(Textures::upgradeButton75, "Media/UI/buttons/upgradeButton75.png");
    mTextures.load(Textures::upgradeButton100, "Media/UI/buttons/upgradeButton100.png");
    mTextures.load(Textures::upgradeButton125, "Media/UI/buttons/upgradeButton125.png");
    mTextures.load(Textures::sellButton, "Media/UI/buttons/SellButton.png");

    // Load construction icon
    mTextures.load(Textures::constructionicon, "Media/Textures/ConstructionIcon.png");

    // Load UI in MenuState
    mTextures.load(Textures::MenuScreen, "Media/Textures/menuScreen.png");
    mTextures.load(Textures::newGameButton, "Media/UI/buttons/newGameButton.png");
    mTextures.load(Textures::loadGameButton, "Media/UI/buttons/loadGameButton.png");
    mTextures.load(Textures::exitButton, "Media/UI/buttons/exitButton.png");

    // Load UI in SettingState
    mTextures.load(Textures::settingButton, "Media/UI/buttons/settingButton.png");
    mTextures.load(Textures::optionsPanelInMenu, "Media/UI/optionsPanel.png");
    mTextures.load(Textures::onButton, "Media/UI/buttons/onButton.png");
    mTextures.load(Textures::offButton, "Media/UI/buttons/offButton.png");
    mTextures.load(Textures::closeButton, "Media/UI/buttons/xButton.png");

    // Load UI in InformationState
    mTextures.load(Textures::infoButton, "Media/UI/buttons/informationButton.png");
    mTextures.load(Textures::infoPanel, "Media/UI/informationPanelInMenuState.png");

    // Load UI in MapSelectionState
    mTextures.load(Textures::mapSelectionBackground, "Media/UI/MapSelectionStateBackground.png");
    mTextures.load(Textures::homeButton, "Media/UI/buttons/homeButton.png");
    mTextures.load(Textures::miniMap1, "Media/UI/miniMapUnlocked/miniMap1.png");
    mTextures.load(Textures::miniMap2, "Media/UI/miniMapUnlocked/miniMap2.png");
    mTextures.load(Textures::miniMap3, "Media/UI/miniMapUnlocked/miniMap3.png");
    mTextures.load(Textures::miniMap4, "Media/UI/miniMapUnlocked/miniMap4.png");
    mTextures.load(Textures::miniMap2Locked, "Media/UI/miniMapLocked/miniMap2Locked.png");
    mTextures.load(Textures::miniMap3Locked, "Media/UI/miniMapLocked/miniMap3Locked.png");
    mTextures.load(Textures::miniMap4Locked, "Media/UI/miniMapLocked/miniMap4Locked.png");
    mTextures.load(Textures::wave, "Media/UI/icons/wave.png");

    // Load UI in PauseState 
    mTextures.load(Textures::pauseButton, "Media/UI/buttons/pauseButton.png");
    mTextures.load(Textures::pausePanel, "Media/UI/pausePanel.png");
    mTextures.load(Textures::restartButton, "Media/UI/buttons/restartButton.png");
    mTextures.load(Textures::quitButton, "Media/UI/buttons/quitButton.png");

    // Load UI in GameState
    mTextures.load(Textures::commingWave, "Media/UI/icons/commingWave.png");
    mTextures.load(Textures::heartIcon, "Media/UI/icons/heartIcon.png");
    mTextures.load(Textures::waveIcon, "Media/UI/icons/waveIcon.png");
    mTextures.load(Textures::currencyIcon, "Media/UI/icons/currencyIcon.png");

    // Load UI in VictoryState
    mTextures.load(Textures::victory, "Media/UI/icons/victory.png");
    mTextures.load(Textures::star, "Media/UI/icons/star.png");

    // Load UI in DefeatState
    mTextures.load(Textures::defeat, "Media/UI/icons/defeat.png");
    mTextures.load(Textures::restartButton2, "Media/UI/buttons/restartButton2.png");
    mTextures.load(Textures::quitButton2, "Media/UI/buttons/quitButton2.png");

    mTextures.load(Textures::powerStation, "Media/Textures/power_station_sprite_sheet.png");
    mTextures.load(Textures::tower1Button, "Media/UI/buttons/tower1Button.png");
    mTextures.load(Textures::tower2Button, "Media/UI/buttons/tower2Button.png");
    mTextures.load(Textures::tower3Button, "Media/UI/buttons/tower3Button.png");
    mTextures.load(Textures::circle, "Media/UI/icons/circle.png");

    // Load Texture in InputNameState 
    mTextures.load(Textures::inputNameBackground, "Media/Textures/inputNameBackground.png");
    mTextures.load(Textures::nextButton, "Media/UI/buttons/nextButton.png");

    // Load UI in SaveMangement for Loading
    mTextures.load(Textures::loadBackground, "Media/UI/loadBackground.png");
    mTextures.load(Textures::nameBar, "Media/UI/buttons/nameBar.png");

    // Load sound and music
    mSoundBuffers.load(SoundBuffers::BulletBomb, "Audio/BulletBomb.wav");
    mSoundBuffers.load(SoundBuffers::BulletFire, "Audio/BulletFire.wav");
    mSoundBuffers.load(SoundBuffers::BulletLaser, "Audio/BulletLaser.wav");

    mSoundBuffers.load(SoundBuffers::EnemyAttack, "Audio/EnemyAttack.wav");
    mSoundBuffers.load(SoundBuffers::EnemyDead, "Audio/EnemyDead.wav");
    mSoundBuffers.load(SoundBuffers::EnemyWalk, "Audio/EnemyWalk.wav");

    mSoundBuffers.load(SoundBuffers::GameOver, "Audio/GameOver.wav");
    mSoundBuffers.load(SoundBuffers::GameWin, "Audio/GameWin.wav");

    mSoundBuffers.load(SoundBuffers::TowerPlace, "Audio/TowerPlace.wav");
    mSoundBuffers.load(SoundBuffers::TowerUpgrade, "Audio/TowerUpgrade.wav");

    mSoundBuffers.load(SoundBuffers::UIclick, "Audio/UIclick.wav");
    mMusics.open(Musics::MusicGame, "Audio/MusicGame.wav");
    mMusics.open(Musics::MusicMenu, "Audio/MusicMenu.wav");

    // Load collision effect
    mTextures.load(Textures::BombImpact, "Media/Textures/BombImpact.png");
    mTextures.load(Textures::FireImpact, "Media/Textures/FireImpact.png");
    mTextures.load(Textures::IceImpact, "Media/Textures/IceImpact.png");
}

void Application::run()
{
    sf::Clock clock;

    while (mWindow.isOpen())
    {
        sf::Time dt = clock.restart(); // Get elapsed time since last frame
        dt = dt * gameSpeed; // Apply game speed multiplier
        processInput();
        update(dt);
        render();
    }
}

void Application::registerStates()
{
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<PauseState>(States::Pause);
    mStateStack.registerState<SettingState>(States::Setting);
    mStateStack.registerState<InformationState>(States::Information);
    mStateStack.registerState<MapSelectionState>(States::MapSelection);
    mStateStack.registerState<VictoryState>(States::Victory);
    mStateStack.registerState<DefeatState>(States::Defeat);
    mStateStack.registerState<InputNameState>(States::InputName);
    mStateStack.registerState<SaveManagement>(States::Load);
}

void Application::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        // Global click sound logic
        if (event.type == sf::Event::MouseButtonPressed) {
            if (isSoundOn) {
                mClickSound.setBuffer(mSoundBuffers.get(SoundBuffers::UIclick));
                mClickSound.play();
            }
        }

        mStateStack.handleEvent(event);

        if (event.type == sf::Event::Closed)
            mWindow.close();
    }
}

void Application::update(sf::Time dt)
{
    mStateStack.update(dt);

    if (mStateStack.isEmpty())
        mWindow.close();
}

void Application::render()
{
    mWindow.clear();
    mStateStack.draw();
    mWindow.display();
}