#pragma once

#include "State.h"

#include <SFML/Graphics.hpp>

class VictoryState : public State
{
public:
	VictoryState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);


private:
	sf::Clock				starTimer; // for star animation
	sf::Sprite				victorySprite;
	sf::Sprite				star;
	sf::Sprite				restartButton;
	sf::Sprite				quitButton;

	int						stars;
	int						starsShown; // number of stars currently being shown

private:
	sf::Sound GameWinSound;
};
