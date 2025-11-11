#include "SaveManagement.h"
#include "Utility.h"
#include "Foreach.h"
#include "ResourceHolder.h"

#include <filesystem>
#include <sstream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

namespace fs = filesystem;

vector<SaveManagement::levelResult> SaveManagement::playerResult(4);
string SaveManagement::playerName = "";

SaveManagement::SaveManagement(StateStack& stack, Context context)
	: State(stack, context)
	, mOptions()
	, mOptionIndex(0)
{
	Texture& texture = context.textures->get(Textures::loadBackground);
	Font& font = context.fonts->get(Fonts::RobotTraffic);

	mBackgroundSprite.setTexture(texture);

	// Get files saved
	scanSaveFolder();

	// Display player names
	int maxSlots = 6;
	for (int i = 0; i < maxSlots; ++i)
	{
		Text name;
		name.setFont(font);
		name.setCharacterSize(40);

		if (i < static_cast<int>(savedNames.size()))
			name.setString(savedNames[i]);
		else
			name.setString("Empty");

		centerOrigin(name);
		mOptions.push_back(name);
	}

	// Clone panel button
	closePanelButton.setTexture(context.textures->get(Textures::closeButton));
	closePanelButton.setPosition(1570.f, 200.f);
	centerOrigin(closePanelButton);

	// Name bar
	Sprite nameBar1;
	nameBar1.setPosition(740.f, 360.f);
	nameBar.push_back(nameBar1);

	Sprite nameBar2;
	nameBar2.setPosition(1300.f, 360.f);
	nameBar.push_back(nameBar2);

	Sprite nameBar3;
	nameBar3.setPosition(740.f, 560.f);
	nameBar.push_back(nameBar3);

	Sprite nameBar4;
	nameBar4.setPosition(1300.f, 560.f);
	nameBar.push_back(nameBar4);

	Sprite nameBar5;
	nameBar5.setPosition(740.f, 760.f);
	nameBar.push_back(nameBar5);

	Sprite nameBar6;
	nameBar6.setPosition(1300.f, 760.f);
	nameBar.push_back(nameBar6);

	for (auto& nb : nameBar) {
		nb.setTexture(context.textures->get(Textures::nameBar));
		centerOrigin(nb);
	}

	layoutNamesOnBars();
}

void SaveManagement::layoutNamesOnBars()
{
	for (size_t i = 0; i < mOptions.size() && i < nameBar.size(); ++i) {
		float barWidth = nameBar[i].getGlobalBounds().width * 0.85f;

		while (mOptions[i].getLocalBounds().width > barWidth && mOptions[i].getCharacterSize() > 12)
			mOptions[i].setCharacterSize(mOptions[i].getCharacterSize() - 1);

		centerOrigin(mOptions[i]);
		mOptions[i].setPosition(nameBar[i].getPosition());
	}
}

void SaveManagement::draw()
{
	RenderWindow& window = *getContext().window;
	Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);

	// Name bar
	for (auto& nb : nameBar) {
		if (nb.getGlobalBounds().contains(mousePos))
			nb.setScale(1.1f, 1.1f);
		else
			nb.setScale(1.f, 1.f);
		window.draw(nb);
	}

	// Text
	FOREACH(const Text & text, mOptions)
		window.draw(text);

	// Close panel button
	if (closePanelButton.getGlobalBounds().contains(mousePos))
		closePanelButton.setScale(1.3f, 1.3f);
	else
		closePanelButton.setScale(1.2f, 1.2f);
	window.draw(closePanelButton);
}

bool SaveManagement::update(Time dt)
{
	return true;
}

bool SaveManagement::handleEvent(const Event& event)
{
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
		Vector2f mousePos = getContext().window->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));

		if (closePanelButton.getGlobalBounds().contains(mousePos)) {
			requestStackPop();
			requestStackPush(States::Menu);
			return true;
		}

		for (size_t i = 0; i < nameBar.size() && i < mOptions.size(); ++i) {
			if (nameBar[i].getGlobalBounds().contains(mousePos) && mOptions[i].getString() != "Empty") {
				mOptionIndex = static_cast<int>(i);
				SaveManagement::setName(mOptions[i].getString());

				SaveManagement::load(SaveManagement::playerName);
				MenuState::isNewPlayer = false;
				requestStackPop();
				requestStackPush(States::MapSelection);
				return true;
			}
		}
	}

	return true;
}

void SaveManagement::scanSaveFolder()
{
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>

	namespace fs = filesystem;

	savedNames.clear();
	if (!fs::exists("saves")) fs::create_directory("saves");

	// Temporary vector to store pairs of (file_path, last_write_time)
	vector<pair<fs::path, fs::file_time_type>> fileEntries;

	for (auto& p : fs::directory_iterator("saves")) {
		if (p.is_regular_file() && p.path().extension() == ".txt") {
			fileEntries.emplace_back(p.path(), fs::last_write_time(p));
		}
	}

	// Sort by last_write_time (oldest first)
	sort(fileEntries.begin(), fileEntries.end(),
		[](const auto& a, const auto& b) {
			return a.second > b.second;
		});

	// Push sorted names into savedNames
	for (const auto& entry : fileEntries) {
		savedNames.push_back(entry.first.stem().string());
	}

}

bool SaveManagement::load(string pName)
{
	ifstream fin("saves/" + pName + ".txt");
	if (!fin) return false;

	string line;

	for (int i = 0; i < 4; i++)
	{
		getline(fin, line);
		stringstream ss(line);
		string temp;

		getline(ss, temp, ' ');
		SaveManagement::playerResult[i].win = stoi(temp);

		getline(ss, temp, ' ');
		SaveManagement::playerResult[i].status = stoi(temp);

		getline(ss, temp, ' ');
		SaveManagement::playerResult[i].stars = stoi(temp);
		if (SaveManagement::playerResult[i].status == -1)
		{
			getline(ss, temp, ' ');
			SaveManagement::playerResult[i].health = stoi(temp);

			getline(ss, temp, ' ');
			SaveManagement::playerResult[i].curWave = stoi(temp);

			getline(ss, temp, ' ');
			SaveManagement::playerResult[i].curGold = stoi(temp);

			SaveManagement::playerResult[i].towers.clear();
			while (getline(ss, temp, ' '))
			{
				ctower t;
				t.setType(stoi(temp));

				getline(ss, temp, ' '); int x = stoi(temp);
				getline(ss, temp, ' '); int y = stoi(temp);

				t.setLocation(cpoint(x, y, 1));
				SaveManagement::playerResult[i].towers.push_back(t);
			}
		}
	}

	return true;
}

void SaveManagement::save(const string playerName)
{
	ofstream fout("saves/" + playerName + ".txt");
	if (!fout) return;

	for (int i = 0; i < 4; i++)
	{
		fout << SaveManagement::playerResult[i].win << " "
			<< SaveManagement::playerResult[i].status << " "
			<< SaveManagement::playerResult[i].stars << " "
			<< SaveManagement::playerResult[i].health << " "
			<< SaveManagement::playerResult[i].curWave << " "
			<< SaveManagement::playerResult[i].curGold << " ";
		for (int j = 0; j < SaveManagement::playerResult[i].towers.size(); j++)
		{
			fout << SaveManagement::playerResult[i].towers[j].getType() << " "
				<< SaveManagement::playerResult[i].towers[j].getLocation().getRow() << " "
				<< SaveManagement::playerResult[i].towers[j].getLocation().getCol() << " ";
		}

		fout << endl;
	}

	fout.close();
}

void SaveManagement::setName(string pName)
{
	SaveManagement::playerName = pName;
}

#include "SaveManagement.h"
