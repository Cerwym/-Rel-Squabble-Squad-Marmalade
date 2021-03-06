#ifndef _GAMEPLAYSTATE_H
#define _GAMEPLAYSTATE_H

#include "StateEngine.h"
#include "GameState.h"
#include "utils.h"
#include "IwGx.h"
#include "Iw2D.h"
#include "IwResManager.h"
#include "IwSound.h"
#include "SoundEffect.h"
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
	void CheckCollisions(const int &pCharacter);
	void CheckObjects(const int &pCharacter);
	void SpawnCharacters();
	void ScrollCranes(const double dt);
	void PlayEff(CIwSoundSpec* spec, CIwSoundInst* inst);

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
	int buttonSoundCount;

	int screenWidth;
	int screenHeight;

	bool m_MouseClicked;
	bool m_SpacePressed;

	Sprite* m_throwingTarget;
	float m_ThrowingTargetSide;
	Sprite* n_guiButtons[2];
	Sprite* characters[3];
	Sprite* m_Portraits[3];
	Sprite* m_PortraitsNot[3];
	Sprite* m_Layers[4];
	GameObject* m_Cranes[3];
	float m_LayerVals[4];
	CIwSoundSpec* m_PortraitSounds[3];
	CIwSoundInst* m_PortraitSoundInsts[3];
	GameObject* m_activeTerminal;

	int m_CharacterIndex;
	// if the level has a terminal, is it active and therefore, has its child class been instantiated
	bool m_isTermActive;
	bool m_isThrowing;
	bool m_canThrow;
	bool m_gameOver;
	bool m_Moving;

	CIwSoundSpec* terminalSound;
	CIwSoundInst* terminalInst;
	CIwSoundSpec* doorSound;
	CIwSoundInst* doorSoundInst;

	SoundEffect* buttonSound;
	SoundEffect* m_TerminalSound;
	SoundEffect* m_ThrowingSound;
	SoundEffect* m_ThrowingNigelSound;

	bool TEMP_HASPLAYED;

	CIwFVec2 m_ClickLocation;

	Camera* m_Cam;
	TileMap* m_Level;
};

#endif