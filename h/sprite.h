#ifndef SPRITE_H
#define SPRITE_H

#include <math.h>
#include <iostream>
#include "Iw2D.h"
#include "camera.h"
#include "Collider.h"

#define GRAVITY 0.278f
#define JUMP_HEIGHT 25.f

class Sprite
{
public:
	Sprite(const char* fname, bool collFlag);
	~Sprite();
	
	inline void SetCenter(const CIwSVec2& center) { m_Center = center; }
	inline CIwFVec2 GetPosition() { return m_Position; }
	inline void SetPosition(const CIwFVec2& position) { m_Position = position; }
	inline void MoveBy(const CIwSVec2& position) { m_Position += CIwFVec2(static_cast<float>(position.x), static_cast<float>(position.y));}
	inline void SetMovSpeed(const CIwFVec2& speed) { m_MovSpeed = speed; }
	// past in a const reference to CIWVec2 instead of the class itself
	inline CIwFVec2 Sprite::LerpTo(const CIwFVec2& end, float scalar)
	{
		CIwFVec2 t = (m_Position + (end - m_Position) * scalar);
		m_Position = t;
		return t;
	}

	void BuildCollision(const char* fname);
	void BuildCollision(CIwImage img);
	CIwImage GetCollisionMap() const {return m_CollisionMap;}
	void SetMaterial();

	inline iwangle GetRotation() { return m_Angle; }
	inline void SetRotation(const iwangle& angle) { m_Angle = angle; }

	inline float GetHeight(){return m_Height;}
	inline float GetWidth(){return m_Width;}

	bool hasCollider(){ return m_hasCollider;}

	void SetVelocity(float v){m_yVel = v;}
	float GetVelocity(){return m_yVel;}
	
	void SetAnimated(const bool animated, float speed, CIwFVec2 frameCount = CIwFVec2());
	void Update(const float deltaTime);
	void Draw();
	void Draw(CIwSVec2& camPos);
	void UpdateCollider();
	bool isColliding(const CIwFVec2& other);
	bool isColliding(Sprite* other);

	void Debug_PrintPos();
	bool ShowColliderPos;

	bool TEMP_ISFALLING;
	bool TEMP_JUSTJUMPED;
	bool TEMP_LANDEDJUMP;
	bool TEMP_ISCOLLIDING;

protected:

private:
	CIwSVec2 m_Center; // The center of the image to rotate around
	CIwFVec2 m_Position; // Screen position
	CIwFVec2 m_MovSpeed;
	
	iwangle m_Angle;
	bool m_Animated;
	bool m_hasCollider;
	float m_AnimSpeed;
	float m_CurrentFrame;
	float m_yVel;
	float m_Width, m_Height;
	const char* m_Name;
	float TEMP_BEFOREJUMPY;
	int TEMP_BOUNCECOUNT;
	CIwSVec2 TEMP_colliderPos;
	/* 
		The difference between CIWsVec2 and CIwFVec2
		CIwSvec2 takes in a 16bit bit signed integer (int16)
		CIwFVec2 takes in floating point numbers (float)
	*/
	CIwSVec2 m_FrameSize;
	CIwSVec2 m_FrameCount;
	CIw2DImage* m_Image;
	Collider* m_Collider;
	CIwImage m_CollisionMap; // 8bit version of the image's alpha channel.

};

#endif