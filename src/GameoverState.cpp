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
	m_Transmanager = new TransitionManager();
	m_TransitionState = FADE_IN;
	m_mouseClicked = false;
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

	if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
		m_mouseClicked = true;
	else
		m_mouseClicked = false;
}

void GameoverState::Update(StateEngine* state, double dt)
{
}

void GameoverState::Draw(StateEngine* state)
{
	if (m_TransitionState == FADE_IN)
	{
		m_Transmanager->TransitionIn(m_menuImage->GetImage(), state->m_deltaTime + 3.5);
		{
			if (m_mouseClicked)
			{
				m_Transmanager->Init();
				m_TransitionState = FADE_OUT;
			}
		}
	}

	if (m_TransitionState == FADE_OUT)
	{
		if (m_Transmanager->TransitionOut(m_menuImage->GetImage(), state->m_deltaTime + 3.5))
		{
			m_Transmanager->Destroy();
			state->ChangeState(MainMenuState::Instance());
		}
	}
}