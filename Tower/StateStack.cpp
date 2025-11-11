#include "StateStack.h"
#include "MapSelectionState.h"
#include "FOREACH.h"

#include <cassert>
#include <iostream>

StateStack::StateStack(State::Context context)
	: mStack()
	, mPendingList()
	, mContext(context)
	, mFactories()
{
}

void StateStack::update(sf::Time dt)
{
	// Iterate from top to bottom, stop as soon as update() returns false
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::draw()
{
	// Draw all active states from bottom to top
	FOREACH(State::Ptr & state, mStack)
		state->draw();
}

void StateStack::handleEvent(const sf::Event& event)
{
	// Iterate from top to bottom, stop as soon as handleEvent() returns false
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(event))
			break;
	}

	applyPendingChanges();
}

void StateStack::pushState(States::ID stateID)
{
	mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
	mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
	return mStack.empty();
}

State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());

	return found->second();
}

// ============ For debug ============= /
std::string getActionName(StateStack::Action action) {
	switch (action) {
	case StateStack::Push:  return "Push";
	case StateStack::Pop:   return "Pop";
	case StateStack::Clear: return "Clear";
	default:                return "Unknown";
	}
}

std::string getStateName(States::ID id) {
	switch (id) {
	case States::None:				return "None";
	case States::Menu:				return "Menu";
	case States::InputName:			return "InputName";
	case States::MapSelection:		return "MapSelection";
	case States::Setting:			return "Setting";
	case States::Information:		return "Infomation";
	case States::Game:				return "Game";
	case States::Load:			    return "Loading";
	case States::Pause:				return "Pause";
	case States::Victory:			return "Victory";
	case States::Defeat:			return "Defeat";
	default:						return "Unknown";
	}
}
// ====================================== //

void StateStack::applyPendingChanges()
{
	FOREACH(PendingChange change, mPendingList)
	{
		switch (change.action)
		{
		case Push:
			mStack.push_back(createState(change.stateID));
			break;

		case Pop:
			mStack.pop_back();
			break;

		case Clear:
			mStack.clear();
			break;
		}
	}

	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
	: action(action)
	, stateID(stateID)
{
}
