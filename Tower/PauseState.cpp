#include "PauseState.h"
#include "Utility.h"
#include "ResourceHolder.h"
#include "Application.h"
#include "SaveManagement.h"
#include "MapSelectionState.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


PauseState::PauseState(StateStack& stack, Context context)
	: State(stack, context)
{
	sf::Vector2f viewSize = context.window->getView().getSize();

	// Pause panel
	pausePanel.setTexture(context.textures->get(Textures::pausePanel));
	pausePanel.setPosition(1040.f, 540.f);
	centerOrigin(pausePanel);

	// Close panel
	closePanel.setTexture(context.textures->get(Textures::closeButton));
	closePanel.setPosition(1440.f, 330.f);
	centerOrigin(closePanel);

	// Restart button
	restartButton.setTexture(context.textures->get(Textures::restartButton));
	restartButton.setPosition(1040.f, 660.f);
	centerOrigin(restartButton);

	// Quit button
	quitButton.setTexture(context.textures->get(Textures::quitButton));
	quitButton.setPosition(1040.f, 760.f);
	centerOrigin(quitButton);

	// On / Off button (for music background & sound effect)
	musicOnButton.setTexture(context.textures->get(Textures::onButton));
	musicOnButton.setPosition(1140.f, 540.f);
	centerOrigin(musicOnButton);

	musicOffButton.setTexture(context.textures->get(Textures::offButton));
	musicOffButton.setPosition(1140.f, 540.f);
	centerOrigin(musicOffButton);

	soundEffectOnButton.setTexture(context.textures->get(Textures::onButton));
	soundEffectOnButton.setPosition(1140.f, 390.f);
	centerOrigin(soundEffectOnButton);

	soundEffectOffButton.setTexture(context.textures->get(Textures::offButton));
	soundEffectOffButton.setPosition(1140.f, 390.f);
	centerOrigin(soundEffectOffButton);
}

void PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	window.setView(window.getDefaultView());

	// Darker screen overlay
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());
	window.draw(backgroundShape);

	// Pause panel
	window.draw(pausePanel);

	// Close panel button
	if (closePanel.getGlobalBounds().contains(mousePos))
		closePanel.setScale(1.1f, 1.1f);
	else
		closePanel.setScale(1.f, 1.f);
	window.draw(closePanel);

	// Restart button
	if (restartButton.getGlobalBounds().contains(mousePos))
		restartButton.setScale(1.1f, 1.1f);
	else
		restartButton.setScale(1.f, 1.f);
	window.draw(restartButton);

	// Quit button
	if (quitButton.getGlobalBounds().contains(mousePos))
		quitButton.setScale(1.1f, 1.1f);
	else
		quitButton.setScale(1.f, 1.f);
	window.draw(quitButton);

	// On / Off button 
	if (*getContext().isMusicOn) {
		if (musicOnButton.getGlobalBounds().contains(mousePos))
			musicOnButton.setScale(1.5f, 1.5f);
		else
			musicOnButton.setScale(1.4f, 1.4f);
		window.draw(musicOnButton);
	}
	else {
		if (musicOffButton.getGlobalBounds().contains(mousePos))
			musicOffButton.setScale(1.5f, 1.5f);
		else
			musicOffButton.setScale(1.4f, 1.4f);
		window.draw(musicOffButton);
	}

	if (*getContext().isSoundOn) {
		if (soundEffectOnButton.getGlobalBounds().contains(mousePos))
			soundEffectOnButton.setScale(1.5f, 1.5f);
		else
			soundEffectOnButton.setScale(1.4f, 1.4f);
		window.draw(soundEffectOnButton);
	}
	else {
		if (soundEffectOffButton.getGlobalBounds().contains(mousePos))
			soundEffectOffButton.setScale(1.5f, 1.5f);
		else
			soundEffectOffButton.setScale(1.4f, 1.4f);
		window.draw(soundEffectOffButton);
	}
}

bool PauseState::update(sf::Time)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		// Convert mouse position from screen pixels to world coordinates (considering the current view)
		sf::Vector2f mousePos = getContext().window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

		if (closePanel.getGlobalBounds().contains(mousePos)) {
			requestStackPop();  // Exit PauseState
			return true;
		}

		// Click MUSIC ON/OFF
		if (musicOnButton.getGlobalBounds().contains(mousePos))
		{
			auto& globalMusicFlag = *getContext().isMusicOn;
			auto& musics = getContext().musics->get(Musics::MusicGame);
			auto& musicState = *getContext().currentMusic;

			if (globalMusicFlag)
			{
				globalMusicFlag = false;
				if (musics.getStatus() == sf::Music::Playing)
				{
					musics.stop();
				}
			}
			else
			{
				globalMusicFlag = true;
				if (musics.getStatus() != sf::Music::Playing)
				{
					musics.setLoop(true);
					musics.play();
				}
			}

			return true;
		}

		// Click SOUND ON/OFF
		if (soundEffectOnButton.getGlobalBounds().contains(mousePos)) {
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

		// Restart button
		if (restartButton.getGlobalBounds().contains(mousePos))
		{
			requestStateClear();

			// Reset saved progress
			int tCurLevel = MapSelectionState::levelID;
			if (!SaveManagement::playerResult[tCurLevel].win)
				SaveManagement::playerResult[tCurLevel].status = 0;
			SaveManagement::playerResult[tCurLevel].stars = 0;
			SaveManagement::playerResult[tCurLevel].health = 0;
			SaveManagement::playerResult[tCurLevel].curWave = 0;
			SaveManagement::playerResult[tCurLevel].curGold = 0;
			SaveManagement::playerResult[tCurLevel].towers.clear();
			SaveManagement::save(SaveManagement::playerName);

			requestStackPush(States::Game); // Back to current Game
			return true;
		}

		// Quit button
		if (quitButton.getGlobalBounds().contains(mousePos)) {
			requestStateClear();
			requestStackPush(States::MapSelection); // Back to MapSelection
			return true;
		}
	}

	return false;
}