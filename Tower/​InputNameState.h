#pragma once

#include "SaveManagement.h"
#include "State.h"
#include "Utility.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

using namespace sf;

class InputNameState : public State
{
public:
	InputNameState(StateStack& stack, Context context);

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);

private:
	Sprite				mBackgroundSprite;
	Sprite				closePanelButton;
	Sprite				nextButton;
	Text				inputName;
	Text				invalidInputName;
	bool				invalidName;
};

