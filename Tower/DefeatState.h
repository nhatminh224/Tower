#pragma once

#include "State.h"

#include <SFML/Graphics.hpp>

class DefeatState : public State
{
public:
	DefeatState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);


private:
	sf::Sprite				defeatSprite;
	sf::Sprite				restartButton;
	sf::Sprite				quitButton;

private:
	sf::Sound				GameOverSound;
};
