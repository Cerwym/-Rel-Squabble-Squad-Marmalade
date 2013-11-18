#ifndef _MAINMENUSTATE_H
#define _MAINMENUSTATE_H

#include "GameState.h"
#include "Iw2D.h"
#include "s3e.h"
#include "IwResManager.h"
#include "sprite.h"

class MainMenuState : public GameState
{
public:
	void Init();
	void Destroy();

	void Pause();
	void Resume();

	void HandleEvent(StateEngine* state);
	void Update(StateEngine* state, double dt);
	void Draw(StateEngine* state);

	static MainMenuState* Instance()
	{
		return &m_MainMenuState;
	}

protected:
	MainMenuState(){} // When called, the state MUST call its own Init() function.

private:
	static MainMenuState m_MainMenuState;
	CIwFVec2 m_Position;
	CIwSVec2 m_Size;

	Sprite* m_menuImage;
};

#endif