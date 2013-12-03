#include "SoundEffect.h"

SoundEffect::SoundEffect(CIwSoundSpec* file)
{
	m_spec = file;
	m_Instance = m_spec->Play(0);
	m_Instance->Stop();
	m_hasPlayed = false;
}

SoundEffect::SoundEffect(const char* file)
{
	m_spec = static_cast<CIwSoundSpec*>(IwGetResManager()->GetResNamed(file, "CIwSoundSpec"));
	m_Instance = m_spec->Play(0);
	m_Instance->Stop();
	m_hasPlayed = false;
}

SoundEffect::~SoundEffect()
{
	m_Instance = NULL;
	if (m_spec)
		delete m_spec;
	m_spec = NULL;
}

void SoundEffect::Play()
{
	if (!m_Instance->IsPlaying() && m_hasPlayed == false)
	{
		m_spec->Play();
		m_hasPlayed = true;
	}
}

void SoundEffect::ResetCounter()
{
	m_hasPlayed = false;
}


