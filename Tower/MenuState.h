#pragma once

#include "State.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio/Sound.hpp>

class MenuState : public State
{
public:
	MenuState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);


private:
	sf::Sprite				mBackgroundSprite;
	std::vector<sf::Sprite> mOptionSprites;

public:
	static bool isNewPlayer;
};
