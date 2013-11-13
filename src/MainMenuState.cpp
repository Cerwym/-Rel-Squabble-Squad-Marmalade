#include <stdio.h>
#include "StateEngine.h"
#include "GameState.h"
#include "MainMenuState.h" // The current state
#include "GameplayState.h" // The state to change to
#include "utils.h"

MainMenuState MainMenuState::m_MainMenuState;

void MainMenuState::Init()
{
	IwGetResManager()->LoadGroup("mainmenusprites.group");
	m_menuImage = Iw2DCreateImageResource("menu_image");
	printf("MainMenuState initialized\n");
}

void MainMenuState::Destroy()
{
	printf("MainMenuState Destroyed\n");
}

void MainMenuState::Pause()
{

}

void MainMenuState::Resume()
{

}

void MainMenuState::HandleEvent(StateEngine* state)
{

	if ( (s3eKeyboardGetState(s3eKeySpace) & S3E_POINTER_STATE_DOWN))
	{
		SleepFor(2);
		state->ChangeState(GameplayState::Instance());
	}
}

void MainMenuState::Update(StateEngine* state, double dt)
{
}

void MainMenuState::Draw(StateEngine* state)
{
	Iw2DDrawImage(m_menuImage, CIwSVec2(0,0));
}