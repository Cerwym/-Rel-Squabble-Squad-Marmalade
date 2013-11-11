#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include "StateEngine.h"

class GameState
{
public:
	virtual void Init() = 0;
	virtual void Destroy() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvent(StateEngine* state) = 0;
	virtual void Update(StateEngine* state, double dt) = 0;
	virtual void Draw(StateEngine* state) = 0;

	// Point to a state, then switch to it
	void ChangeState(StateEngine* engine, GameState* state)
	{
		engine->ChangeState(state);
	}

protected:
	GameState(){}
};

#endif