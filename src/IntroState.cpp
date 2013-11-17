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

	m_SplashImg1 = Iw2DCreateImageResource("logo");
	m_SplashImg2 = Iw2DCreateImageResource("splash2");

	m_TransManager.Init();
	m_TransitionState == FADE_IN;

	if (s3eAudioIsCodecSupported(S3E_AUDIO_CODEC_MP3))
		s3eAudioPlay("audio\\music.mp3", 1);

	printf("IntroState initialized\n");
}

void IntroState::Destroy()
{
	IwGetResManager()->DestroyGroup("Intro");
	delete m_SplashImg1;
	delete m_SplashImg2;
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
	if ( (s3eKeyboardGetState(s3eKeySpace) & S3E_POINTER_STATE_DOWN))
		state->ChangeState(MainMenuState::Instance());
}

void IntroState::Update(StateEngine* state, double dt)
{
	
}

void IntroState::Draw(StateEngine* state)
{
	if (m_TransitionState == FADE_IN)
		if (m_TransManager.TransitionIn(m_SplashImg1, state->m_deltaTime + 3.5))
		{
			m_TransitionState = BETWEEN;
			m_TransManager.Init();
			std::cout << "State changed to between" << std::endl;
		}

	if (m_TransitionState == BETWEEN)
		if (m_TransManager.TransitionBetween(m_SplashImg1, m_SplashImg2, state->m_deltaTime + 3.5))
		{
			m_TransitionState = FADE_OUT;
			m_TransManager.Init();
		}

	if (m_TransitionState == FADE_OUT)
		if (m_TransManager.TransitionOut(m_SplashImg2, state->m_deltaTime + 3.5))
		{
			m_TransManager.FinishTransition();
			state->ChangeState(MainMenuState::Instance());//std::cout << "Finished transitioning" << std::endl;
		}
}
