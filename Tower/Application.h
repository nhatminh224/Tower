#pragma once

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "Player.h"
#include "StateStack.h"

#include <SFML/Graphics/RenderWindow.hpp>

class Application
{
public:
    Application(); // Constructor: Initializes the application
    void run(); // Main game loop entry point

private:
    // Handles all user inputs (keyboard, mouse, etc.)
    void processInput();

    // Updates game logic based on elapsed time (delta time)
    void update(sf::Time dt);

    // Draws everything to the screen
    void render();

    // Registers all possible game states into the state stack
    void registerStates();

    sf::RenderWindow mWindow;
    TextureHolder mTextures;
    FontHolder mFonts;
    Player mPlayer;
    int mVictoryStars;
    StateStack mStateStack;

    MusicState currentMusic = MusicState::None;
    SoundBufferHolder mSoundBuffers;
    MusicHolder mMusics;

    float gameSpeed = 1.0f;

    bool isMusicOn = true;
    bool isSoundOn = true;

private:
    sf::Sound mClickSound;
};
