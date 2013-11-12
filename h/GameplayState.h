#ifndef _GAMEPLAYSTATE_H
#define _GAMEPLAYSTATE_H

#include "StateEngine.h"
#include "GameState.h"
#include "IwGx.h"
#include "Iw2D.h"
#include "IwResManager.h"
#include "sprite.h"
#include "GameObject.h"
#include "camera.h"
#include "TileMap.h"
#include "s3e.h"

class GameplayState : public GameState
{
public:
	// Gamestate Virtual functions
	void Init();
	void Destroy();

	void Pause();
	void Resume();

	void HandleEvent(StateEngine* state);
	void Update(StateEngine* state, double dt);
	void Draw(StateEngine* state);

	static GameplayState* Instance()
	{
		return &m_GameplayState;
	}
public:
	// New functions
	void CheckInterations(StateEngine* state);

protected:
	GameplayState(){} // When called, the state MUST call its own Init() function.

private:
	static GameplayState m_GameplayState;
	
	CIwFVec2 m_Position;
	CIwSVec2 m_Size;

	uint64 m_LastUpdate;

	int m_TimeToOneSec;
	int m_CountUpdates;
	int m_UpdatesPerSec;

	int screenWidth;
	int screenHeight;

	bool m_MouseClicked;
	bool m_SpacePressed;
	bool m_UpPressed;

	Sprite* m_throwingTarget;
	Sprite* n_guiButtons[2];
	Sprite* characters[3];
	Sprite* m_Portraits[3];
	GameObject* m_activeTerminal;

	CIw2DFont* m_Font;

	int m_CharacterIndex;
	// if the level has a terminal, is it active and therefore, has its child class been instantiated
	bool m_isTermActive;
	bool m_isThrowing;
	bool m_canThrow;

	Camera* m_Cam;
	TileMap* m_Level;
};

#endif