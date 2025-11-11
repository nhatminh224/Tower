#pragma once
#include "State.h"
#include "clevel.h"
#include "MenuState.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;

class SaveManagement : public State
{
public:
	SaveManagement(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(Time dt);
	virtual bool			handleEvent(const Event& event);

private:
	Sprite				mBackgroundSprite;
	Sprite				closePanelButton;
	vector<Sprite>		nameBar;

	vector<Text>		mOptions;
	size_t				mOptionIndex;

private:
	// process load file
	vector<string>		savedNames;
	void				scanSaveFolder();
	void				layoutNamesOnBars();

public:

	// process save file
	struct levelResult
	{
		bool win = false;	// ever win? 
		int status;			// isGameOver = 0, isGameWin = 1, isNotFinished = -1
		int curGold;
		int stars;			// (status != 1) => (stars = 0)
		int health;
		int curWave;
		vector<ctower> towers;
	};
	static vector<levelResult> playerResult;
	static void save(const string playerName);

	// Modify player's name
	static string playerName;
	static void setName(string pName);

	static bool load(string pName);
};

