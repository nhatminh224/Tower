#include "InputNameState.h"

InputNameState::InputNameState(StateStack& stack, Context context)
	: State(stack, context)
	, mBackgroundSprite()
	, inputName()
	, invalidInputName()
	, invalidName(false)
{
	Texture& texture = context.textures->get(Textures::inputNameBackground);
	mBackgroundSprite.setTexture(texture);

	Font& font = context.fonts->get(Fonts::BruceForever);
	context.window->setKeyRepeatEnabled(false); // input name only happens once

	closePanelButton.setTexture(context.textures->get(Textures::closeButton));
	closePanelButton.setPosition(1400.f, 310.f);
	centerOrigin(closePanelButton);

	nextButton.setTexture(context.textures->get(Textures::nextButton));
	nextButton.setPosition(1020.f, 660.f);
	centerOrigin(nextButton);

	inputName.setFont(font);
	inputName.setString(SaveManagement::playerName);
	inputName.setCharacterSize(70);
	centerOrigin(inputName);
	inputName.setPosition(0.5f * texture.getSize().x, 0.5f * texture.getSize().y);

	invalidInputName.setFont(font);
	invalidInputName.setString("Input name can contain only 9 characters!");
	invalidInputName.setFillColor(Color::Green);
	invalidInputName.setCharacterSize(40);
	invalidInputName.setPosition(960.f, 1000.f);
	centerOrigin(invalidInputName);
}

void InputNameState::draw()
{
	RenderWindow& window = *getContext().window;
	Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
	window.setView(window.getDefaultView());

	// Draw background
	window.draw(mBackgroundSprite);

	// Update and draw player name input
	inputName.setString(SaveManagement::playerName);
	FloatRect bounds = inputName.getLocalBounds();
	inputName.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
	inputName.setPosition(1020.f, 530.f);
	centerOrigin(inputName);
	window.draw(inputName);

	// Check if input name is valid
	if (invalidName == true)
		window.draw(invalidInputName);

	// Close panel button
	if (closePanelButton.getGlobalBounds().contains(mousePos))
		closePanelButton.setScale(1.1f, 1.1f);
	else
		closePanelButton.setScale(1.f, 1.f);
	window.draw(closePanelButton);

	// Next button
	if (nextButton.getGlobalBounds().contains(mousePos))
		nextButton.setScale(1.1f, 1.1f);
	else
		nextButton.setScale(1.f, 1.f);
	window.draw(nextButton);
}

bool InputNameState::update(Time)
{
	return true;
}

bool InputNameState::handleEvent(const Event& event)
{
	// If is new player: Input name before choosing level
	if (MenuState::isNewPlayer && event.type == Event::TextEntered)
	{
		const uint32_t u = event.text.unicode;

		// Backspace || Delete
		if ((u == 8 || u == 127) && !SaveManagement::playerName.empty())
			SaveManagement::playerName.pop_back();

		// Skip Enter/Return/Newline
		else if (u == 13 || u == 10) {

		}

		else {
			char ch = static_cast<char>(u);
			if (std::isprint(static_cast<unsigned char>(ch)) && SaveManagement::playerName.size() < 15)
				SaveManagement::playerName.push_back(ch);
		}
	}

	if (SaveManagement::playerName.size() < 10)
		invalidName = false;
	else
		invalidName = true;

	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
		Vector2f mousePos = getContext().window->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));

		if (closePanelButton.getGlobalBounds().contains(mousePos)) {
			requestStackPop();
			requestStackPush(States::Menu);
			return true;
		}

		if (nextButton.getGlobalBounds().contains(mousePos) && !SaveManagement::playerName.empty() && !invalidName) {
			getContext().window->setKeyRepeatEnabled(true);
			MenuState::isNewPlayer = false;
			requestStackPop();
			requestStackPush(States::MapSelection);
			return true;
		}
	}

	return true;
}