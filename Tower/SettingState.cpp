#include "SettingState.h"
#include "Utility.h"
#include "Foreach.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

SettingState::SettingState(StateStack& stack, Context context)
	: State(stack, context)
{
	// Set background
	sf::Texture& texture = context.textures->get(Textures::optionsPanelInMenu);
	mBackgroundSprite.setTexture(texture);

	// On / Off Button (for music background and sound effect)
	mSoundOnButton.setTexture(context.textures->get(Textures::onButton));
	mSoundOnButton.setPosition(1200.f, 420.f);
	centerOrigin(mSoundOnButton);

	mSoundOffButton.setTexture(context.textures->get(Textures::offButton));
	mSoundOffButton.setPosition(1200.f, 420.f);
	centerOrigin(mSoundOffButton);

	mMusicOnButton.setTexture(context.textures->get(Textures::onButton));
	mMusicOnButton.setPosition(1200.f, 620.f);
	centerOrigin(mMusicOnButton);

	mMusicOffButton.setTexture(context.textures->get(Textures::offButton));
	mMusicOffButton.setPosition(1200.f, 620.f);
	centerOrigin(mMusicOffButton);

	// Close Options Panel Button
	mCloseOptionsPanelButton.setTexture(context.textures->get(Textures::closeButton));
	mCloseOptionsPanelButton.setPosition(1400.f, 290.f);
	centerOrigin(mCloseOptionsPanelButton);
}

void SettingState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);

	window.draw(mBackgroundSprite);

	// Sound button
	if (*getContext().isSoundOn) {
		if (mSoundOnButton.getGlobalBounds().contains(mousePos))
			mSoundOnButton.setScale(1.8f, 1.8f);
		else
			mSoundOnButton.setScale(1.7f, 1.7f);
		window.draw(mSoundOnButton);
	}

	else {
		if (mSoundOffButton.getGlobalBounds().contains(mousePos))
			mSoundOffButton.setScale(1.8f, 1.8f);
		else
			mSoundOffButton.setScale(1.7f, 1.7f);
		window.draw(mSoundOffButton);
	}

	// Music button
	if (*getContext().isMusicOn) {
		if (mMusicOnButton.getGlobalBounds().contains(mousePos))
			mMusicOnButton.setScale(1.8f, 1.8f);
		else
			mMusicOnButton.setScale(1.7f, 1.7f);
		window.draw(mMusicOnButton);
	}

	else {
		if (mMusicOffButton.getGlobalBounds().contains(mousePos))
			mMusicOffButton.setScale(1.8f, 1.8f);
		else
			mMusicOffButton.setScale(1.7f, 1.7f);
		window.draw(mMusicOffButton);
	}

	if (mCloseOptionsPanelButton.getGlobalBounds().contains(mousePos))
		mCloseOptionsPanelButton.setScale(1.2f, 1.2f);
	else
		mCloseOptionsPanelButton.setScale(1.1f, 1.1f);
	window.draw(mCloseOptionsPanelButton);
}

bool SettingState::update(sf::Time)
{
	return true;
}

bool SettingState::handleEvent(const sf::Event& event)
{

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		// Convert mouse position from screen pixels to world coordinates (considering the current view)
		sf::Vector2f mousePos = getContext().window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

		// Click MUSIC ON/OFF 
		if (mMusicOnButton.getGlobalBounds().contains(mousePos) ||
			mMusicOffButton.getGlobalBounds().contains(mousePos))
		{
			auto& globalMusicFlag = *getContext().isMusicOn;
			auto& menuMusic = getContext().musics->get(Musics::MusicMenu);

			if (globalMusicFlag)
			{
				globalMusicFlag = false;
				if (menuMusic.getStatus() == sf::Music::Playing)
				{
					menuMusic.stop();
				}
			}
			else
			{
				globalMusicFlag = true;
				if (menuMusic.getStatus() != sf::Music::Playing)
				{
					menuMusic.setLoop(true);
					menuMusic.play();
				}
			}

			return true;
		}
		// Click SOUND ON/OFF 
		if (mSoundOnButton.getGlobalBounds().contains(mousePos) ||
			mSoundOffButton.getGlobalBounds().contains(mousePos))
		{
			auto& globalSoundFlag = *getContext().isSoundOn;

			if (globalSoundFlag)
			{
				globalSoundFlag = false;
			}
			else
			{
				globalSoundFlag = true;
			}

			return true;
		}

		if (mCloseOptionsPanelButton.getGlobalBounds().contains(mousePos)) {
			requestStackPop();
			requestStackPush(States::Menu);
		}
	}

	return true;
}
