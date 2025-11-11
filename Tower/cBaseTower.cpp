#include "cBaseTower.h"

#include <iostream>

cBaseTower::cBaseTower() :
    maxHealth(20),
    currentHealth(maxHealth),
    mRecentlyDamaged(false),
    position(0, 0, 0)
{
    // Initialize health bars
    setHealthBarSize(100.f, 10.f);
    healthBarBackground.setFillColor(sf::Color::Red);
    healthBarFill.setFillColor(sf::Color::Green);
    updateSpritePosition();
}

void cBaseTower::takeDamage(int damage) {
    // Reduce current health
    currentHealth -= damage;
    if (currentHealth < 0) currentHealth = 0; // Prevent going below 0

    // Mark as recently damaged and restart the damage timer
    mDamageTimer.restart();
    mRecentlyDamaged = true;

    // Update health bar size according to remaining health
    float healthPercentage = static_cast<float>(currentHealth) / maxHealth;
    healthBarFill.setSize(sf::Vector2f(
        healthBarBackground.getSize().x * healthPercentage,
        healthBarBackground.getSize().y
    ));
}

void cBaseTower::heal(int amount) {
    // Increase current health
    currentHealth += amount;
    if (currentHealth > maxHealth) currentHealth = maxHealth; // Cap at max HP

    // Update health bar size according to new health value
    float healthPercentage = static_cast<float>(currentHealth) / maxHealth;
    healthBarFill.setSize(sf::Vector2f(
        healthBarBackground.getSize().x * healthPercentage,
        healthBarBackground.getSize().y
    ));
}

bool cBaseTower::isDestroyed() const {
    return currentHealth <= 0;
}

bool cBaseTower::recentlyDamaged() const {
    // Return true if tower was damaged in the last 0.5 seconds
    return mRecentlyDamaged && mDamageTimer.getElapsedTime().asSeconds() < 0.5f;
}

void cBaseTower::setTexture(const sf::Texture& texture) {
    sprite.setTexture(texture);
    // Center the sprite origin if needed
    sprite.setOrigin(
        sprite.getLocalBounds().width / 2,
        sprite.getLocalBounds().height / 2
    );
    sprite.setScale(1.8f, 1.8f);
}

void cBaseTower::setPosition(int row, int col) {
    // Set grid-based position and update sprite/health bar placement
    position = cpoint(row, col, 0);
    updateSpritePosition();
}

void cBaseTower::setPixelPosition(float x, float y) {
    // Convert pixel coordinates to grid position, then update placement
    position = cpoint::fromXYToRowCol(static_cast<int>(x), static_cast<int>(y));
    updateSpritePosition();
}

void cBaseTower::setHealth(int hp) {
    currentHealth = hp;
}

void cBaseTower::setMaxHealh(int hp)
{
    maxHealth = hp;
}

void cBaseTower::setCurrentHealth(int curHP)
{
    this->currentHealth = curHP;
}

void cBaseTower::setHealthBarSize(float width, float height) {
    healthBarBackground.setSize(sf::Vector2f(width, height));
    healthBarFill.setSize(sf::Vector2f(width, height));
    updateSpritePosition(); // Reposition with new size
}

void cBaseTower::drawHealthBar(sf::RenderWindow& window) const {
    window.draw(healthBarBackground);
    window.draw(healthBarFill);
}

void cBaseTower::updateSpritePosition() {
    float pixelX = position.getPixelX();
    float pixelY = position.getPixelY();

    sprite.setPosition(pixelX, pixelY);

    // Position health bar above the tower
    healthBarBackground.setPosition(
        pixelX - healthBarBackground.getSize().x / 2 - 20.f,
        pixelY - 170.f // Offset above the tower
    );
    healthBarFill.setPosition(
        healthBarBackground.getPosition().x,
        healthBarBackground.getPosition().y
    );
}