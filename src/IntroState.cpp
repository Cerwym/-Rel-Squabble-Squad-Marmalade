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

	m_Logo = Iw2DCreateImageResource("logo");

	if (s3eAudioIsCodecSupported(S3E_AUDIO_CODEC_MP3))
		s3eAudioPlay("audio\\music.mp3", 1);

	printf("IntroState initialized\n");
}

void IntroState::Destroy()
{
	delete m_Logo;
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
	if ( (s3eKeyboardGetState(s3eKeySpace) & S3E_POINTER_STATE_DOWN))
	{
		SleepFor(1);
		state->ChangeState(MainMenuState::Instance());
	}
}

void IntroState::Update(StateEngine* state, double dt)
{
	//m_rotation += dt * 50;
}

void IntroState::Draw(StateEngine* state)
{
	Iw2DDrawImage(m_Logo, CIwSVec2(0,0));
}