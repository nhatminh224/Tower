#pragma once
#include "State.h"
#include "MenuState.h"
#include "SaveManagement.h" 
#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics.hpp>

class MapSelectionState : public State
{
public:
    MapSelectionState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

private:
    sf::Sprite				                        backgroundSprite;
    sf::Sprite                                      homeButton;

    std::vector<std::vector<sf::Sprite>>            starIcons;
    std::vector<std::pair<sf::Sprite, sf::Text>>    waveInfo;
    std::vector<sf::Sprite>                         unlockedLevels;
    std::vector<sf::Sprite>                         lockedLevels;

public:
    static int levelID;

private:
    Font                                            font;
    vector<sf::Text>                                currentWave;
    sf::Text                                        playerName;
};


