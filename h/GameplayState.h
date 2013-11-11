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
	void DrawTouchButtons();
	// Draw a sprite, centered at the specified position with a given size
	void DrawSpriteCentered(CIw2DImage*, int, int, int);
	void DrawText(int16, int16);
	void CheckInterations();

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

	int TEMP_charIndex;
	bool TEMP_termActive;
	bool TEMP_isThrowing;
	bool m_canThrow;
	CIwFVec2 TEMP_target;

	Camera* m_Cam;
	TileMap* m_Level;
};

#endif