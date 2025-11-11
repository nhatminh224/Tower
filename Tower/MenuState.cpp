#include "MenuState.h"
#include "Utility.h"
#include "Foreach.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

bool MenuState::isNewPlayer = true;

MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
{
	// Set background
	sf::Texture& texture = context.textures->get(Textures::MenuScreen);
	mBackgroundSprite.setTexture(texture);

	// Load Music
	auto& musicHolder = *getContext().musics;
	auto& musicFlag = *getContext().isMusicOn;
	auto& musicState = *getContext().currentMusic;

	// Stop game music if it was playing
	if (musicState == MusicState::Game) {
		musicHolder.get(Musics::MusicGame).stop();
	}
	if (musicFlag) {
		auto& menuMusic = musicHolder.get(Musics::MusicMenu);
		menuMusic.setLoop(true);
		menuMusic.setVolume(10);
		menuMusic.play();
		musicState = MusicState::Menu;
	}

	// New Game Button
	sf::Sprite newGameSprite;
	newGameSprite.setTexture(context.textures->get(Textures::newGameButton));
	centerOrigin(newGameSprite);
	newGameSprite.setPosition(980.f, 640.f);
	mOptionSprites.push_back(newGameSprite);

	// Load Game Button
	sf::Sprite loadGameSprite;
	loadGameSprite.setTexture(context.textures->get(Textures::loadGameButton));
	centerOrigin(loadGameSprite);
	loadGameSprite.setPosition(980.f, 760.f);
	mOptionSprites.push_back(loadGameSprite);

	// Exit Button
	sf::Sprite exitSprite;
	exitSprite.setTexture(context.textures->get(Textures::exitButton));
	centerOrigin(exitSprite);
	exitSprite.setPosition(980.f, 880.f);
	mOptionSprites.push_back(exitSprite);

	// Setting button
	sf::Sprite mSettingsButton;
	mSettingsButton.setTexture(context.textures->get(Textures::settingButton));
	mSettingsButton.setPosition(1800.f, 60.f);
	centerOrigin(mSettingsButton);
	mOptionSprites.push_back(mSettingsButton);

	// Info button
	sf::Sprite mInfoButton;
	mInfoButton.setTexture(context.textures->get(Textures::infoButton));
	mInfoButton.setPosition(1680.f, 60.f);
	centerOrigin(mInfoButton);
	mOptionSprites.push_back(mInfoButton);
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);

	for (std::size_t i = 0; i < mOptionSprites.size(); ++i)
	{
		// Apply hover scale directly to the original sprite 
		if (mOptionSprites[i].getGlobalBounds().contains(mousePos))
			mOptionSprites[i].setScale(1.1f, 1.1f); // Enlarge when hovered
		else
			mOptionSprites[i].setScale(1.f, 1.f); // Reset when not hovered

		window.draw(mOptionSprites[i]);
	}
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		// Convert mouse position from screen pixels to world coordinates (considering the current view)
		sf::Vector2f mousePos = getContext().window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

		for (std::size_t i = 0; i < mOptionSprites.size(); ++i) {
			if (mOptionSprites[i].getGlobalBounds().contains(mousePos)) {
				if (i == 0) {
					requestStackPop();
					if (MenuState::isNewPlayer)
						requestStackPush(States::InputName); // New game
					else
						requestStackPush(States::MapSelection); // New game
				}

				else if (i == 1) {
					requestStackPop();
					requestStackPush(States::Load);
				}

				else if (i == 2)
					requestStackPop(); // Exit

				else if (i == 3) {
					requestStackPop();
					requestStackPush(States::Setting); // Open setting panel
				}

				else if (i == 4) {
					requestStackPop();
					requestStackPush(States::Information); // Open information panel
				}

				break;
			}
		}
	}

	return true;
}

