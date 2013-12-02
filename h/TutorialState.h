#ifndef _TUTORIALSTATE_H
#define _TUTORIALSTATE_H

#include "GameState.h"
#include "Iw2D.h"
#include "s3e.h"
#include "IwResManager.h"
#include "TransitionManager.h"
#include "sprite.h"

class TutorialState : public GameState
{
public:
	void Init();
	void Destroy();

	void Pause();
	void Resume();

	void HandleEvent(StateEngine* state);
	void Update(StateEngine* state, double dt);
	void Draw(StateEngine* state);

	static TutorialState* Instance()
	{
		return &m_TutorialState;
	}

protected:
	TutorialState(){} // When called, the state MUST call its own Init() function.

private:
	static TutorialState m_TutorialState;

	CIwFVec2 m_Position;
	CIwSVec2 m_Size;

	Sprite* m_Splash1;
	Sprite* m_Splash2;
	TransitionManager m_TransManager;

	bool m_mouseClicked;

	enum TStat{FADE_IN = 0, BETWEEN = 1, FADE_OUT = 2}; TStat m_TransitionState;

};

#endif