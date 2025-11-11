#pragma once
#include "cpoint.h"
#include "cbullet.h"
#include <SFML/Graphics.hpp>
#include "FrameAnimator.h"

using namespace sf;
using namespace std;

class ctower {
private:
    Sprite _sprite;
    cpoint _location;
    cbullet _cb;
    float _shootTimer;
    int _targetEnemyIdx;
    int _type;

    // MainTower
    Sprite _mainTowerSprite;
    Texture* _mainTowerTexture;
    Vector2f _mainTowerPos;
    int _mainTowerHealth;

    // Animation 
    Sprite _effectSprite;
    FrameAnimator _effectAnim;
    bool _effectPlaying;

public:
    ctower();

    int calcPathBullet();
    void init(const Texture& tex, float x, float y, int index, int itower);
    void resetShootTimer() { _shootTimer = 0.f; } // Reset the shoot timer to 0 after firing a bullet
    void addShootTimer(float dt) { _shootTimer += dt; } // Add delta time to the shoot timer, used to track cooldown between shots
    void changeOrigin(int index, int itower, const Texture& tex); // UI tower

    // shootEffect
    void initEffect(const Texture& tex, int frameWidth, int frameHeight, int totalFrames, float animSpeed);
    void startEffect();
    void updateEffect(float deltaTime);
    bool isEffectPlaying() const { return _effectPlaying; }

    // Getter
    int getTargetEnemyIdx() const { return _targetEnemyIdx; }
    int getType() const { return _type; } // Tower 1 = 0 ...
    cbullet& getBullet() { return _cb; }
    cpoint getLocation() const { return _location; }
    float getShootTimer() const { return _shootTimer; } // Get the current value of the shoot timer to check if the tower is ready to shoot
    const Sprite& getSprite() const { return _sprite; }
    const Sprite& getEffectSprite() const { return _effectSprite; }
    const Sprite& getMainTowerSprite() const { return _mainTowerSprite; }
    int getHealth() { return _mainTowerHealth; }

    // Setter
    void setTargetEnemyIdx(int idx) { _targetEnemyIdx = idx; }
    void setMapForBullet(cpoint map[][cpoint::MAP_COL]) { _cb.updateMap(map); }
    void setType(int n) { _type = n; }
    void setLocation(const cpoint& loc) { _location = loc; }
    void setHealth(int health) { _mainTowerHealth = health; }
};
#pragma once
