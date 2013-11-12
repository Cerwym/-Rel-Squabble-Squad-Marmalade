#include <stdio.h>
#include "StateEngine.h"
#include "GameState.h"
#include "GameoverState.h" // The current state
#include "MainMenuState.h" // The state to change to
#include "utils.h"

GameoverState GameoverState::m_GameOverState;

void GameoverState::Init()
{
	IwGetResManager()->LoadGroup("gameoversprites.group");
	m_menuImage = Iw2DCreateImageResource("game_over");
	printf("GameoverState initialized\n");
}

void GameoverState::Destroy()
{
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
	Iw2DDrawImage(m_menuImage, CIwSVec2(0,0));
}