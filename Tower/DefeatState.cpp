#include "DefeatState.h"
#include "Utility.h"
#include "Foreach.h"
#include "ResourceHolder.h"
#include "Application.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

DefeatState::DefeatState(StateStack& stack, Context context)
	: State(stack, context)
{
	defeatSprite.setTexture(context.textures->get(Textures::defeat));
	defeatSprite.setPosition(960.f, 460);
	centerOrigin(defeatSprite);

	restartButton.setTexture(context.textures->get(Textures::restartButton2));
	restartButton.setPosition(830.f, 680.f);
	centerOrigin(restartButton);

	quitButton.setTexture(context.textures->get(Textures::quitButton2));
	quitButton.setPosition(1100.f, 680.f);
	centerOrigin(quitButton);

	if (*getContext().isSoundOn)
	{
		GameOverSound.setBuffer(getContext().soundBuffers->get(SoundBuffers::GameOver));
		GameOverSound.setVolume(60);
		GameOverSound.play();
	}
}

void DefeatState::draw() {
	sf::RenderWindow& window = *getContext().window;
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	window.setView(window.getDefaultView());

	// Darker screen overlay
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());
	window.draw(backgroundShape);

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

	// Defeat sprite
	window.draw(defeatSprite);
}

bool DefeatState::update(sf::Time dt) {
	return false;
}

bool DefeatState::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		// Convert mouse position from screen pixels to world coordinates (considering the current view)
		sf::Vector2f mousePos = getContext().window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

		// Restart button
		if (restartButton.getGlobalBounds().contains(mousePos)) {
			requestStateClear(); // Clear all states including VictoryState & GameState
			requestStackPush(States::Game); // Push fresh GameState
			return true;
		}

		// Quit button
		if (quitButton.getGlobalBounds().contains(mousePos)) {
			requestStateClear(); // Clear all states including VictoryState & GameState
			requestStackPush(States::MapSelection);
			return true;
		}
	}
	return false;
}
