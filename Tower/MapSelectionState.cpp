#include "MapSelectionState.h"
#include "Utility.h"
#include "Foreach.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

int MapSelectionState::levelID = 0;

MapSelectionState::MapSelectionState(StateStack& stack, Context context)
	: State(stack, context)
{
	// Set background
	sf::Texture& texture = context.textures->get(Textures::mapSelectionBackground);
	backgroundSprite.setTexture(texture);

	// Load homebutton texture
	homeButton.setTexture(context.textures->get(Textures::homeButton));
	homeButton.setPosition(1840.0f, 90.0f);
	centerOrigin(homeButton);

	// Load unlocked map textures
	sf::Sprite level1;
	level1.setTexture(context.textures->get(Textures::miniMap1));
	level1.setPosition(580.0f, 360.0f);
	centerOrigin(level1);
	unlockedLevels.push_back(level1);

	sf::Sprite level2;
	level2.setTexture(context.textures->get(Textures::miniMap2));
	level2.setPosition(1380.0f, 360.0f);
	centerOrigin(level2);
	unlockedLevels.push_back(level2);

	sf::Sprite level3;
	level3.setTexture(context.textures->get(Textures::miniMap3));
	level3.setPosition(580.0f, 800.0f);
	centerOrigin(level3);
	unlockedLevels.push_back(level3);

	sf::Sprite level4;
	level4.setTexture(context.textures->get(Textures::miniMap4));
	level4.setPosition(1380.0f, 800.0f);
	centerOrigin(level4);
	unlockedLevels.push_back(level4);

	// Load locked map textures (map 1 is always unlocked)
	sf::Sprite level2Locked;
	level2Locked.setTexture(context.textures->get(Textures::miniMap2Locked));
	level2Locked.setPosition(1380.0f, 360.0f);
	centerOrigin(level2Locked);
	lockedLevels.push_back(level2Locked);

	sf::Sprite level3Locked;
	level3Locked.setTexture(context.textures->get(Textures::miniMap3Locked));
	level3Locked.setPosition(580.0f, 800.0f);
	centerOrigin(level3Locked);
	lockedLevels.push_back(level3Locked);

	sf::Sprite level4Locked;
	level4Locked.setTexture(context.textures->get(Textures::miniMap4Locked));
	level4Locked.setPosition(1380.0f, 800.0f);
	centerOrigin(level4Locked);
	lockedLevels.push_back(level4Locked);

	// Load Text shown for 4 maps
	font = getContext().fonts->get(Fonts::BruceForever);

	for (int i = 0; i < 4; i++)
	{
		if (SaveManagement::playerResult[i].status == -1) // not done playing
		{
			string temp = to_string(SaveManagement::playerResult[i].curWave + 1);

			Text waveText;
			waveText.setFont(font);
			waveText.setCharacterSize(40);
			waveText.setFillColor(Color::Yellow);
			currentWave.push_back(waveText);

			sf::Sprite waveIcon;
			waveIcon.setTexture(context.textures->get(Textures::wave));
			waveIcon.setScale(1.3f, 1.3f);

			if (i == 0)
			{
				waveText.setString(temp + "/" + to_string(3));
				waveText.setPosition(560.0f, 550.0f);
				waveIcon.setPosition(480.0f, 550.0f);
			}
			else if (i == 1)
			{
				waveText.setString(temp + "/" + to_string(3));
				waveText.setPosition(1360.0f, 550.0f);
				waveIcon.setPosition(1280.0f, 550.0f);
			}
			else if (i == 2)
			{
				waveText.setString(temp + "/" + to_string(4));
				waveText.setPosition(560.0f, 990.0f);
				waveIcon.setPosition(480.0f, 990.0f);
			}
			else if (i == 3)
			{
				waveText.setString(temp + "/" + to_string(5));
				waveText.setPosition(1360.0f, 990.f);
				waveIcon.setPosition(1280.0f, 990.0f);
			}

			waveInfo.push_back({ waveIcon, waveText });
		}

		else if (SaveManagement::playerResult[i].status == 1) // done playing
		{
			int starCount = SaveManagement::playerResult[i].stars;
			std::vector<sf::Sprite> starsForLevel;

			if (starCount > 0)
			{
				const float scale = 0.7f;
				const float spacing = 12.f;
				const float offsetY = 220.f;

				sf::Sprite proto;
				proto.setTexture(context.textures->get(Textures::star));
				proto.setScale(scale, scale);

				auto lb = proto.getLocalBounds();
				float w = lb.width * scale;
				float h = lb.height * scale;

				sf::Vector2f center = unlockedLevels[i].getPosition();
				center.y += offsetY;

				// Compute the total width of the star row, then left-most x to center it
				float totalW = starCount * w + (starCount - 1) * spacing;
				float startX = center.x - totalW * 0.5f;

				for (int s = 0; s < starCount; ++s)
				{
					sf::Sprite starSprite = proto;
					starSprite.setOrigin(lb.width * 0.5f, lb.height * 0.5f);

					float x = startX + s * (w + spacing) + w * 0.5f;
					starSprite.setPosition(x, center.y);
					starsForLevel.push_back(starSprite);
				}
			}

			starIcons.push_back(std::move(starsForLevel));
		}
	}

	// Set up text to display name
	playerName.setFont(font);
	playerName.setCharacterSize(50);
	playerName.setFillColor(Color::White);
	playerName.setPosition(80.f, 45.f);
	playerName.setString(SaveManagement::playerName);
}

void MapSelectionState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	window.setView(window.getDefaultView());
	window.draw(backgroundSprite);
	window.draw(playerName);

	// Home button
	if (homeButton.getGlobalBounds().contains(mousePos))
		homeButton.setScale(1.1f, 1.1f);
	else
		homeButton.setScale(1.f, 1.f);
	window.draw(homeButton);

	// 4 Mini maps 
	int currentWaveIndex = 0;
	int starsIndex = 0;

	for (int i = 0; i < 4; i++)
	{
		if (i == 0)			// Map 1: always unlocked
		{
			if (unlockedLevels[i].getGlobalBounds().contains(mousePos))
				unlockedLevels[i].setScale(1.1f, 1.1f);
			else
				unlockedLevels[i].setScale(1.f, 1.f);

			window.draw(unlockedLevels[i]);

			if (SaveManagement::playerResult[i].status == -1)
			{
				window.draw(waveInfo[currentWaveIndex].first);   // icon wave
				window.draw(waveInfo[currentWaveIndex].second);  // text wave
				currentWaveIndex++;
			}

			else if (SaveManagement::playerResult[i].status == 1)
			{
				for (auto& s : starIcons[starsIndex])
					window.draw(s);
				starsIndex++;
			}
			continue;
		}

		else
		{
			if (!(SaveManagement::playerResult[i - 1].win)) // Locked: previous level is not done
			{
				if (lockedLevels[i - 1].getGlobalBounds().contains(mousePos))
					lockedLevels[i - 1].setScale(1.1f, 1.1f);
				else
					lockedLevels[i - 1].setScale(1.f, 1.f);

				window.draw(lockedLevels[i - 1]);
			}
			else // Unlocked
			{

				if (unlockedLevels[i].getGlobalBounds().contains(mousePos))
					unlockedLevels[i].setScale(1.1f, 1.1f);
				else
					unlockedLevels[i].setScale(1.f, 1.f);

				window.draw(unlockedLevels[i]);

				if (SaveManagement::playerResult[i].status == -1)
				{
					window.draw(waveInfo[currentWaveIndex].first);   // icon wave
					window.draw(waveInfo[currentWaveIndex].second);  // text wave
					currentWaveIndex++;
				}

				else if (SaveManagement::playerResult[i].status == 1)
				{
					for (auto& s : starIcons[starsIndex])
						window.draw(s);
					starsIndex++;
				}
			}
		}
	}
}

bool MapSelectionState::update(sf::Time dt)
{
	return true;
}

bool MapSelectionState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		// Convert mouse position from screen pixels to world coordinates (considering the current view)
		sf::Vector2f mousePos = getContext().window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

		// Click home button
		if (homeButton.getGlobalBounds().contains(mousePos))
		{
			requestStackPop();
			requestStackPush(States::Menu);
		}

		// Choose level
		for (size_t i = 0; i < unlockedLevels.size(); i++) {
			if (i == 0 || SaveManagement::playerResult[i - 1].win) // check if last level was done
			{
				if (unlockedLevels[i].getGlobalBounds().contains(mousePos))
				{
					levelID = i;
					requestStackPop();
					requestStackPush(States::Game);
				}
			}
		}
	}

	return true;
}
