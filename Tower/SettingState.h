#pragma once

#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class SettingState : public State
{
public:
	SettingState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);

private:
	sf::Sprite				mBackgroundSprite;
	sf::Sprite				mSoundOnButton;
	sf::Sprite				mSoundOffButton;
	sf::Sprite				mMusicOnButton;
	sf::Sprite				mMusicOffButton;
	sf::Sprite				mCloseOptionsPanelButton;
};

#pragma once
