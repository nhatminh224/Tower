// TowerDefense\Player.h
#pragma once

#include <SFML/Window/Event.hpp>
#include <map>

class CommandQueue;

class Player
{
public:
    enum Action
    {
        PlaceTower,
        CancelAction,
        ActionCount
    };

public:
    Player();

    void handleEvent(const sf::Event& event);
    void handleRealtimeInput();

    // For Tower Defense, track mouse position for tower placement
    void setMousePosition(float x, float y);
    sf::Vector2f getMousePosition() const;

private:
    // Mouse position for tower placement
    sf::Vector2f mMousePosition;

    // Add other player state here (money, selected tower type, etc.)
    int mMoney;
    int mSelectedTowerType;

public:
    int getMoney() const;
    void addMoney(int amount);
    bool spendMoney(int amount);
    void setMoney(int amount);

    // Other player-related methods...
};