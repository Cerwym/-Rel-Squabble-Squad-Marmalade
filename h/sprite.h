#ifndef SPRITE_H
#define SPRITE_H

#include <math.h>
#include "Iw2D.h"

#define GRAVITY 0.678

class Sprite
{
public:
	Sprite(const char* image);
	~Sprite();
	
	inline void SetCenter(const CIwSVec2& center) { m_Center = center; }
	inline CIwFVec2 GetPosition() { return m_Position; }
	inline void SetPosition(const CIwFVec2& position) { m_Position = position; }
	inline void SetMovSpeed(const CIwFVec2& speed) { m_MovSpeed = speed; }
	// past in a const reference to CIWVec2 instead of the class itself
	inline CIwFVec2 Sprite::LerpTo(const CIwFVec2& end, float scalar)
	{
		return (m_Position + (end - m_Position) * scalar);
	}

	void SetMaterial();

	inline iwangle GetRotation() { return m_Angle; }
	inline void SetRotation(const iwangle& angle) { m_Angle = angle; }
	
	void SetAnimated(const bool animated, float speed, CIwFVec2 frameCount = CIwFVec2());
	void Update(const float deltaTime);
	void Draw();
	bool isColliding(Sprite* const);

	void Debug_PrintPos();

	bool TEMP_ISFALLING;

protected:
	inline float GetHeight();
	inline float GetWidth();

private:
	CIwFVec2 m_Center; // The centre of the image to rotate around
	CIwFVec2 m_Position; // Screen position
	CIwFVec2 m_MovSpeed;
	
	iwangle m_Angle;
	bool m_Animated;
	float m_AnimSpeed;
	float m_CurrentFrame;
	float m_yVel;
	/* 
		The difference between CIWsVec2 and CIwFVec2
		CIwSvec2 takes in a 16bit bit signed integer (int16)
		CIwFVec2 takes in floating point numbers (float)
	*/
	CIwSVec2 m_FrameSize;
	CIwSVec2 m_FrameCount;
	CIw2DImage* m_Image;

};

#endif