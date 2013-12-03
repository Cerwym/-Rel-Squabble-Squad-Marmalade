#ifndef _SOUNDEFFECT_H
#define _SOUNDEFFECT_H

#include "IwSound.h"
class SoundEffect
{
public:
	SoundEffect(CIwSoundSpec* file);
	SoundEffect(const char* file);
	~SoundEffect();

	void Play();
	void ResetCounter();
	bool isPlaying(){return m_Instance->IsPlaying();}
	void SetProperties();

private:

	CIwSoundSpec* m_spec;
	CIwSoundInst* m_Instance;
	bool m_hasPlayed;

};
#endif