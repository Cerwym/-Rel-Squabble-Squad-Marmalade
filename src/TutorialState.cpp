#include <stdio.h>
#include "StateEngine.h"
#include "GameState.h"
#include "TutorialState.h" // The current state
#include "GameplayState.h" // The state to change to
#include "utils.h"

TutorialState TutorialState::m_TutorialState;

void TutorialState::Init()
{
	IwGetResManager()->LoadGroup("tutorialsprites.group");
	m_TransManager.Init();
	m_Splash1 = new Sprite("tutorial_page_1", false);
	m_Splash2 = new Sprite("tutorial_page_2", false);
	m_mouseClicked = false;
	printf("TutorialState initialized\n");
}

void TutorialState::Destroy()
{
	delete m_Splash1;
	delete m_Splash2;
	IwGetResManager()->DestroyGroup("TutorialScreen");
	printf("TutorialState Destroyed\n");
}

void TutorialState::Pause()
{

}

void TutorialState::Resume()
{

}

void TutorialState::HandleEvent(StateEngine* state)
{

	if ( (s3eKeyboardGetState(s3eKeySpace) & S3E_POINTER_STATE_DOWN))
	{
		SleepFor(2);
		state->ChangeState(GameplayState::Instance());
	}

	if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
		m_mouseClicked = true;
	else
		m_mouseClicked = false;
}

void TutorialState::Update(StateEngine* state, double dt)
{

}

void TutorialState::Draw(StateEngine* state)
{
	if (m_TransitionState == FADE_IN)
	{
		if (m_TransManager.TransitionIn(m_Splash1->GetImage(), state->m_deltaTime + 1.5))
		{
			if (m_mouseClicked)
			{
				m_TransitionState = BETWEEN;
				m_TransManager.Init();
			}
			
		}
	}

	if (m_TransitionState == BETWEEN)
	{
		if (m_TransManager.TransitionBetween(m_Splash1->GetImage(), m_Splash2->GetImage(), state->m_deltaTime + 1.5))
		{
			if (m_mouseClicked)
			{
				m_TransitionState = FADE_OUT;
				m_TransManager.Init();
			}
			
		}
	}

	if (m_TransitionState == FADE_OUT)
	{
		if (m_TransManager.TransitionOut(m_Splash2->GetImage(), state->m_deltaTime + 3.5) )
		{
			m_TransManager.Destroy();
			state->ChangeState(GameplayState::Instance());
		}
	}
}