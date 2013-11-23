#include <stdio.h>
#include <iostream>

#include "StateEngine.h"
#include "GameState.h"
#include "Iw2D.h"
#include "s3e.h"
#include "IwResManager.h"
#include "IwSound.h"


void StateEngine::Init(const char* title)
{
	m_isRunning = true;

	Iw2DInit();
	IwSoundInit();
	IwResManagerInit();

#ifdef IW_BUILD_RESOURCES
	// Tell resource system how to convert WAV files
	IwGetResManager()->AddHandler(new CIwResHandlerWAV);
#endif
	printf("Engine Initialized\n");
}

void StateEngine::Destroy()
{
	IwSoundTerminate();
	// Cleanup the states on the stack
	while (!m_States.empty())
	{
		m_States.back()->Destroy();
		m_States.pop_back();
	}

	IwResManagerTerminate();
	Iw2DTerminate();
	printf("Engine cleaned up\n");
}

void StateEngine::ChangeState(GameState* state)
{
	// Destroy the CURRENT state
	if (!m_States.empty())
	{
		m_States.back()->Destroy();
		m_States.pop_back();
	}

	// Stop the music from playing
	if (s3eAudioIsPlaying())
	s3eAudioStop();

	// Store and init the new state, fresh to the party
	m_States.push_back(state);
	m_States.back()->Init();
}

void StateEngine::PushState(GameState* state)
{
	// 'Pause' the current state
	if (!m_States.empty())
	{
		m_States.back()->Pause();
	}

	// Store and init the new state
	m_States.push_back(state);
	//m_States.back()->Init();
	std::cout << "States existing = " << m_States.size() << std::endl;
}

void StateEngine::PopState()
{
	// DESTROY the current state
	if (!m_States.empty())
	{
		m_States.back()->Destroy();
		m_States.pop_back();
	}

	// Resume the previous state
	if (!m_States.empty()) 
		m_States.back()->Resume();
}

void StateEngine::HandleEvent()
{
	m_States.back()->HandleEvent(this);

	if (s3eDeviceCheckQuitRequest())
		m_isRunning = false; // to exit correctly, applications should poll for quit requests
}

void StateEngine::Update()
{
	IwGetSoundManager()->Update();
	m_LastTime = s3eTimerGetMs();
	m_deltaTime = (m_LastTime - m_CurrTime) / 60;
	m_States.back()->Update(this, m_deltaTime);
	m_CurrTime = m_LastTime;
}

void StateEngine::Draw()
{
	Iw2DSurfaceClear(0xff000000);

	m_States.back()->Draw(this);

	Iw2DSurfaceShow();
}