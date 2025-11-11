#include "ctower.h"

ctower::ctower() : _shootTimer(0.f), _targetEnemyIdx(-1), _mainTowerHealth(5), _mainTowerTexture(nullptr) {}

int ctower::calcPathBullet() {
    return _cb.calcPathBullet(_location);
}

void ctower::init(const Texture& tex, float x, float y, int index, int itower) {
    _sprite.setTexture(tex);
    changeOrigin(index, itower, tex);
    _sprite.setScale(0.4f, 0.4f);
    _sprite.setPosition(x, y);
    _location = cpoint::fromXYToRowCol(x, y);
}

void ctower::changeOrigin(int index, int itower, const Texture& tex)
{
    switch (index) {
    case 0:
        if (itower == 2)
            _sprite.setOrigin(tex.getSize().x / 4.0f, tex.getSize().y / 1.1f);
        else
            _sprite.setOrigin(tex.getSize().x / 3.0f, tex.getSize().y);
        break;
    case 1:
        if (itower == 1 || itower == 4)
            _sprite.setOrigin(tex.getSize().x / 4.0f, tex.getSize().y / 1.1f);
        else
            _sprite.setOrigin(tex.getSize().x / 3.0f, tex.getSize().y / 1.1f);
        break;
    case 2:
        if (itower == 4 || itower == 5)
            _sprite.setOrigin(tex.getSize().x / 4.0f, tex.getSize().y / 1.1f);
        else
            _sprite.setOrigin(tex.getSize().x / 3.0f, tex.getSize().y / 1.1f);
        break;
    case 3:
        if (itower == 0 || itower == 3 || itower == 4)
            _sprite.setOrigin(tex.getSize().x / 4.0f, tex.getSize().y / 1.1f);
        else
            _sprite.setOrigin(tex.getSize().x / 3.0f, tex.getSize().y / 1.1f);
        break;
    default:
        break;
    }
}

void ctower::initEffect(const Texture& tex, int frameW, int frameH, int totalFrames, float animSpeed) {
    _effectSprite.setTexture(tex);
    _effectSprite.setOrigin(frameW / 3.f, frameH);
    _effectSprite.setScale(3.f, 3.f);

    _effectAnim.init(frameW, frameH, animSpeed, totalFrames, /*loop*/ false);
    _effectAnim.applyTo(_effectSprite);

    _effectPlaying = false;
}

void ctower::startEffect() {
    if (_effectPlaying) return;
    _effectPlaying = true;

    _effectAnim.reset();
    _effectAnim.applyTo(_effectSprite);
    _effectSprite.setPosition(_sprite.getPosition().x, _sprite.getPosition().y - 40.f);
}

void ctower::updateEffect(float deltaTime) {
    if (!_effectPlaying) return;

    _effectAnim.update(deltaTime);
    _effectAnim.applyTo(_effectSprite);

    if (_effectAnim.isFinished())
        _effectPlaying = false;
}




