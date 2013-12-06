#include <stdio.h>
#include <iostream>
#include "StateEngine.h"
#include "GameState.h"
#include "IntroState.h"
#include "MainMenuState.h"
#include "utils.h"

IntroState IntroState::m_IntroState;

void IntroState::Init()
{
	IwGetResManager()->LoadGroup("introsprites.group");

	m_Splash1 = new Sprite("logo", false);
	m_Splash2 = new Sprite("splash2", false);

	m_TransManager.Init();
	m_TransitionState = FADE_IN;

	if (s3eAudioIsCodecSupported(S3E_AUDIO_CODEC_MP3))
		s3eAudioPlay("music.mp3", 1);

	printf("IntroState initialized\n");
}

void IntroState::Destroy()
{
	delete m_Splash1;
	delete m_Splash2;
	IwGetResManager()->DestroyGroup("Intro");
	printf("IntroState Destroyed\n");
}

void IntroState::Pause()
{

}

void IntroState::Resume()
{

}

void IntroState::HandleEvent(StateEngine* state)
{
	if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
		state->ChangeState(MainMenuState::Instance());
}

void IntroState::Update(StateEngine* state, double dt)
{
	
}

void IntroState::Draw(StateEngine* state)
{
	if (m_TransitionState == FADE_IN)
	{
		if (m_TransManager.TransitionIn(m_Splash1->GetImage(), state->m_deltaTime + 3.5))
		{
			m_TransitionState = BETWEEN;
			m_TransManager.Init();
		}
	}

	if (m_TransitionState == BETWEEN)
	{
		if (m_TransManager.TransitionBetween(m_Splash1->GetImage(), m_Splash2->GetImage(), state->m_deltaTime + 3.5))
		{
			m_TransitionState = FADE_OUT;
			m_TransManager.Init();
		}
	}

	if (m_TransitionState == FADE_OUT)
	{
		if (m_TransManager.TransitionOut(m_Splash2->GetImage(), state->m_deltaTime + 3.5))
		{
			m_TransManager.Destroy();
			state->ChangeState(MainMenuState::Instance());//std::cout << "Finished transitioning" << std::endl;
		}
	}
}
