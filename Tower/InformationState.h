#pragma once

#include "State.h"
#include <SFML/Graphics.hpp>
#include <vector>

class InformationState : public State
{
public:
    InformationState(StateStack& stack, Context context);
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

private:
    void clampScrollOffset();  // Helper function

    // A fixed "virtual canvas" for UI (same as your design)
    sf::View mUiView;            // 1920x1080 logical view
    sf::FloatRect mUiViewport;        // letterboxed viewport in the window

    sf::Sprite mBackgroundSprite;
    sf::Sprite mCloseInfoButton;
    sf::RectangleShape mScrollArea;

    sf::Font mFont;  // Store font as member
    std::vector<sf::Text> mInfoTexts;

    float mScrollOffset = 0.f;
    float mMaxOffset = 0.f;
    float mLastMouseY = 0.f;

    bool mIsDragging = false;;
};
