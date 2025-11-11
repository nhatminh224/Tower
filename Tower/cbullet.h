#pragma once
#include "cpoint.h"
#include "cenemy.h" 
#include <SFML/Graphics.hpp>
#include "FrameAnimator.h"

class cbullet
{
    // Position
    cpoint _p[cpoint::MAP_ROW * cpoint::MAP_COL];
    cpoint _m[cpoint::MAP_ROW][cpoint::MAP_COL];
    cpoint _curr;

    // Stats
    int _n, _speed;
    float _posX, _posY;
    bool _active;
    int _targetIdx;
    int _damage;

    // Animation for bullet
    sf::Sprite _sprite;
    FrameAnimator _anim;

    // Animation for collision effect
    sf::Sprite _collisionSprite;
    FrameAnimator _collisionAnim;
    bool _collisionPlaying = false;

public:
    cbullet();

    // Update internal copy of map for bullet path calculations
    void updateMap(cpoint map[][cpoint::MAP_COL]);

    // Getter
    cpoint getCurr() const { return _curr; }
    cpoint* getP() { return _p; }
    int getSpeed() const { return _speed; }
    int getN() const { return _n; }
    float getX() const { return _posX; }
    float getY() const { return _posY; }
    int getTargetIdx() const { return _targetIdx; }
    int getDamage() const { return _damage; }
    const sf::Sprite& getSprite() const { return _sprite; }

    // Setter
    void setCurr(const cpoint& tcurr) { _curr = tcurr; }
    void setN(int tn) { if (tn >= 0 && tn <= cpoint::MAP_ROW * cpoint::MAP_COL) _n = tn; }
    void setSpeed(int tspeed) { if (tspeed > 0 && tspeed < 20) _speed = tspeed; }
    void setPosition(float x, float y);
    void setTargetIdx(int idx) { _targetIdx = idx; }
    void setDamage(int dmg) { _damage = dmg; }

    int queryCFromRowCol(int row, int col) const;
    int calcPathBullet(const cpoint& tower);

    // Collision detection
    bool checkCollision(const cenemy& enemy) const;

    // Move bullet one step toward the enemy's current position
    void trackEnemy(const cenemy& enemy, float deltaTime);

    // Position and movement
    void init(const sf::Texture& tex, float x, float y, int frameWidth, int frameHeight, int totalFrames, float animSpeed, float scale);
    void updateAnimation(float deltaTime);
    void updateSprite();
    void move(float dx, float dy);

    // Bullet state management
    bool isActive() const { return _active; } // Returns true if the bullet is still active (on screen, valid target), used to skip deactivated bullets.
    void deactivate() { _active = false; } // Marks the bullet as inactive after hitting an enemy or going off - screen, Prevents further updates or rendering.

    // Animation for collision effect
    const sf::Sprite& getCollisionSprite() const { return _collisionSprite; }
    bool isCollisionPlaying() const { return _collisionPlaying; }
    bool isRemovable() const { return !_active && !_collisionPlaying; } // safe to erase
    void initCollisionEffect(const sf::Texture& tex, int frameWidth, int frameHeight, int totalFrames, float animSpeed, float scale);
    void triggerCollision(float x, float y);
    void updateCollisionEffectAnimation(float deltaTime);
    void updateCollision(float deltaTime);
};
