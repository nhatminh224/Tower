#pragma once
#include <SFML/Graphics.hpp>

class FrameAnimator {
public:
    FrameAnimator();

    void init(int frameWidth, int frameHeight, float frameSpeed, int totalFrames, bool loop = true);
    void update(float deltaTime);
    void applyTo(sf::Sprite& sprite);

    void reset();
    bool isFinished() const;

    // Getter
    int getFrameWidth() const { return _frameWidth; }
    int getFrameHeight() const { return _frameHeight; }

private:
    int _frameWidth;
    int _frameHeight;
    int _totalFrames;
    int _currentFrame;
    float _frameSpeed;
    float _timer;
    bool _loop;
    bool _finished;
    sf::IntRect _frameRect;
};


