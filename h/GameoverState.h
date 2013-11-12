#ifndef _GAMEOVERSTATE_H
#define _GAMEOVERSTATE_H

#include "GameState.h"
#include "Iw2D.h"
#include "s3e.h"
#include "IwResManager.h"

class GameoverState : public GameState
{
public:
	void Init();
	void Destroy();

	void Pause();
	void Resume();

	void HandleEvent(StateEngine* state);
	void Update(StateEngine* state, double dt);
	void Draw(StateEngine* state);

	static GameoverState* Instance()
	{
		return &m_GameOverState;
	}

protected:
	GameoverState(){} // When called, the state MUST call its own Init() function.

private:
	static GameoverState m_GameOverState;
	CIwFVec2 m_Position;
	CIwSVec2 m_Size;

	CIw2DImage* m_menuImage;
};

#endif