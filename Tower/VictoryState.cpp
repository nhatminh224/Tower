#include "VictoryState.h"
#include "Utility.h"
#include "Foreach.h"
#include "ResourceHolder.h"
#include "Application.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

VictoryState::VictoryState(StateStack& stack, Context context)
	: State(stack, context)
	, stars(*context.victoryStars)
	, starsShown(0)
{
	victorySprite.setTexture(context.textures->get(Textures::victory));
	victorySprite.setPosition(960.f, 460);
	centerOrigin(victorySprite);

	restartButton.setTexture(context.textures->get(Textures::restartButton2));
	restartButton.setPosition(830.f, 740.f);
	centerOrigin(restartButton);

	quitButton.setTexture(context.textures->get(Textures::quitButton2));
	quitButton.setPosition(1100.f, 740.f);
	centerOrigin(quitButton);

	star.setTexture(context.textures->get(Textures::star));

	if (*getContext().isSoundOn)
	{
		GameWinSound.setBuffer(getContext().soundBuffers->get(SoundBuffers::GameWin));
		GameWinSound.setVolume(60);
		GameWinSound.play();
	}
}

void VictoryState::draw() {
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

	// Victory sprite
	window.draw(victorySprite);

	//======================= Stars =======================//
	const float starWidth = 100.f;
	const float spacing = 15.f;

	sf::Vector2f bannerCenter = victorySprite.getPosition();
	sf::Vector2f starsOrigin = bannerCenter + sf::Vector2f(70.f, 120.f);  // offset downward from banner

	float totalWidth = stars * starWidth + (stars - 1) * spacing;
	float startX = starsOrigin.x - totalWidth / 2.f;

	for (int i = 0; i < stars; ++i) {
		star.setOrigin(starWidth / 2.f, starWidth / 2.f);
		star.setScale(1.f, 1.f);
		star.setPosition(startX + i * (starWidth + spacing), starsOrigin.y);

		if (i < starsShown)
			star.setColor(sf::Color::White); // Active star (earned)
		else
			star.setColor(sf::Color(255, 255, 255, 60)); // Faded star (not yet shown)

		window.draw(star);
	}
	//===================================================//
}

bool VictoryState::update(sf::Time dt) {
	if (starsShown < stars) {
		// Show next star every 0.5 seconds
		if (starTimer.getElapsedTime().asSeconds() > 0.5f) {
			starsShown++;		 // Reveal one more star
			starTimer.restart(); // Reset the timer for the next star
		}
	}

	return false;
}

bool VictoryState::handleEvent(const sf::Event& event) {
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
