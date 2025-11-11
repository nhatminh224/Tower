#include "State.h"
#include "StateStack.h"


State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player, int& stars, SoundBufferHolder& sfx, MusicHolder& music, bool& musicFlag, bool& sfxFlag, MusicState& musicState)
	: window(&window)
	, textures(&textures)
	, fonts(&fonts)
	, player(&player)
	, victoryStars(&stars)
	, soundBuffers(&sfx)
	, musics(&music)
	, isMusicOn(&musicFlag)
	, isSoundOn(&sfxFlag)
	, currentMusic(&musicState)
{
}

State::State(StateStack& stack, Context context)
	: mStack(&stack)
	, mContext(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}
#include "State.h"
