#include "FrameAnimator.h"

FrameAnimator::FrameAnimator()
    : _frameWidth(0), _frameHeight(0), _totalFrames(1),
    _currentFrame(0), _frameSpeed(0.1f), _timer(0.f),
    _loop(true), _finished(false) {
}

void FrameAnimator::init(int frameWidth, int frameHeight, float frameSpeed, int totalFrames, bool loop) {
    _frameWidth = frameWidth;
    _frameHeight = frameHeight;
    _frameSpeed = frameSpeed;
    _totalFrames = totalFrames;
    _loop = loop;
    _currentFrame = 0;
    _timer = 0.f;
    _finished = false;
    _frameRect = sf::IntRect(0, 0, _frameWidth, _frameHeight);
}

void FrameAnimator::update(float deltaTime) {
    if (_finished) return;

    _timer += deltaTime;
    if (_timer >= _frameSpeed) {
        _timer -= _frameSpeed;
        _currentFrame++;

        if (_currentFrame >= _totalFrames) {
            if (_loop) _currentFrame = 0;
            else {
                _currentFrame = _totalFrames - 1;
                _finished = true;
            }
        }

        _frameRect.left = _currentFrame * _frameWidth;
    }
}

void FrameAnimator::applyTo(sf::Sprite& sprite) {
    sprite.setTextureRect(_frameRect);
}

void FrameAnimator::reset() {
    _currentFrame = 0;
    _timer = 0.f;
    _finished = false;
    _frameRect.left = 0;
}

bool FrameAnimator::isFinished() const {
    return _finished;
}
