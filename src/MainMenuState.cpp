#include <stdio.h>
#include "StateEngine.h"
#include "GameState.h"
#include "MainMenuState.h" // The current state
#include "TutorialState.h" // The state to change to
#include "utils.h"

MainMenuState MainMenuState::m_MainMenuState;

void MainMenuState::Init()
{
	IwGetResManager()->LoadGroup("mainmenusprites.group");
	m_menuImage = new Sprite("menu_image", false);
	printf("MainMenuState initialized\n");
}

void MainMenuState::Destroy()
{
	delete m_menuImage;
	IwGetResManager()->DestroyGroup("MainMenu");
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
	if ( (s3eKeyboardGetState(s3eKeySpace) & S3E_POINTER_STATE_DOWN) || (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
	{
		SleepFor(2);
		state->ChangeState(TutorialState::Instance());
	}
}

void MainMenuState::Update(StateEngine* state, double dt)
{
}

void MainMenuState::Draw(StateEngine* state)
{
	m_menuImage->Draw();
}