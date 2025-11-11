// TowerDefense\Player.cpp
#include "Player.h"
#include "ResourceIdentifiers.h"

Player::Player()
    : mMousePosition(0.f, 0.f)
    , mMoney(GameConstants::STARTING_MONEY)
    , mSelectedTowerType(0)
{
}

void Player::handleEvent(const sf::Event& event)
{
    // Handle events like tower selection, purchases, etc.
    if (event.type == sf::Event::MouseMoved)
    {
        setMousePosition(static_cast<float>(event.mouseMove.x),
            static_cast<float>(event.mouseMove.y));
    }
}

void Player::handleRealtimeInput()
{
    // Handle any continuous input needed
}

void Player::setMousePosition(float x, float y)
{
    mMousePosition.x = x;
    mMousePosition.y = y;
}

sf::Vector2f Player::getMousePosition() const
{
    return mMousePosition;
}

int Player::getMoney() const
{
    return mMoney;
}

void Player::addMoney(int amount)
{
    mMoney += amount;
}

bool Player::spendMoney(int amount)
{
    if (mMoney >= amount) {
        mMoney -= amount;
        return true;
    }
    return false;
}

void Player::setMoney(int amount)
{
    this->mMoney = amount;
}
