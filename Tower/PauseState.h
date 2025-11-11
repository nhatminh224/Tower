#pragma once
#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class PauseState : public State
{
public:
	PauseState(StateStack& stack, Context context);

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	sf::Sprite			pausePanel;
	sf::Sprite			closePanel;
	sf::Sprite			restartButton;
	sf::Sprite			quitButton;
	sf::Sprite			soundEffectOnButton;
	sf::Sprite			soundEffectOffButton;
	sf::Sprite			musicOnButton;
	sf::Sprite			musicOffButton;

	bool				isMusicOn = true;
	bool				isSoundEffectOn = true;
};
