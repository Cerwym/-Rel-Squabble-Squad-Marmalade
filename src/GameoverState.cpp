#include <stdio.h>
#include <iostream>
#include "StateEngine.h"
#include "GameState.h"
#include "GameoverState.h" // The current state
#include "MainMenuState.h" // The state to change to
#include "utils.h"

GameoverState GameoverState::m_GameOverState;

void GameoverState::Init()
{
	IwGetResManager()->LoadGroup("gameoversprites.group");
	m_menuImage = new Sprite("game_over", false);
	m_Transmanager.Init();
	printf("GameoverState initialized\n");
}

void GameoverState::Destroy()
{
	delete m_menuImage;
	IwGetResManager()->DestroyGroup("gameover");
	printf("GameoverState Destroyed\n");
}

void GameoverState::Pause()
{

}

void GameoverState::Resume()
{

}

void GameoverState::HandleEvent(StateEngine* state)
{

	if ( (s3eKeyboardGetState(s3eKeySpace) & S3E_POINTER_STATE_DOWN))
	{
		SleepFor(1.5);
		state->ChangeState(MainMenuState::Instance());
	}
}

void GameoverState::Update(StateEngine* state, double dt)
{
}

void GameoverState::Draw(StateEngine* state)
{
	if (m_Transmanager.TransitionIn(m_menuImage->GetImage(), state->m_deltaTime + 3.5))
		m_menuImage->Draw();
}