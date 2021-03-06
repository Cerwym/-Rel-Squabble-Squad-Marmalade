#ifndef SPRITE_H
#define SPRITE_H

#include <math.h>
#include <iostream>
#include "Iw2D.h"
#include "camera.h"
#include "Collider.h"

#define GRAVITY 6.78f
#define JUMP_HEIGHT 128

enum FACING{FACING_RIGHT = 0, FACING_LEFT = 1};
enum ANIMSTATE{ANIMATION_NONE = 0, ANIMATION_IDLE = 1, ANIMATION_WALKING = 2, ANIMATION_ACTION = 3};

class Sprite
{
public:
	Sprite(const char*, bool);
	Sprite(const char*, bool, CIwFVec2);
	~Sprite();
	
	inline void SetCenter(const CIwSVec2& center) { m_Center = center; }
	inline CIwFVec2 GetPosition() { return m_Position; }
	inline void SetPosition(const CIwFVec2& position) { m_Position = position; }
	inline float GetBottom(){return m_Position.y + m_Height;}
	inline void SetMovSpeed(const CIwFVec2& speed) { m_MovSpeed = speed; }
	inline void ResetPosition() { m_Position = m_LastPosition;}
	inline CIwFVec2 GetMovSpeed() {return m_MovSpeed;}
	// past in a const reference to CIWVec2 instead of the class itself
	inline CIwFVec2 LerpTo(const CIwFVec2& end, float scalar)
	{
		m_LastPosition = m_Position;
		CIwFVec2 t = (m_Position + (end - m_Position) * scalar);
		m_Position = t;
		return t;
	}

	inline int GetDirection(){return m_facingDir;}
	inline void SetDirection(FACING f) {m_facingDir = f;}
	CIw2DImage* GetImage() const {return m_Image;}
	void MoveBy(const CIwFVec2& val, double dt);

	void SetRegion(CIwSVec2& region){m_Region = region;}

	inline iwangle GetRotation() { return m_Angle; }
	inline void SetRotation(const iwangle& angle) { m_Angle = angle; }

	inline float GetHeight(){return m_Height;}
	inline float GetWidth(){return m_Width;}

	CIwFVec2 GetLastPosition(){return m_LastPosition;}
	CIwFVec2 GetLastMovement(){return m_LastMovementVal;}

	bool hasCollider(){ return m_hasCollider;}

	void SetVelocity(float v){m_yVel = v;}
	float GetVelocity(){return m_yVel;}
	
	void SetAnimated(const bool animated, float speed, CIwFVec2& frameCount);
	void Update(const float deltaTime);
	void Draw();
	void Draw(const CIwSVec2& camPos);
	void AddCollider(float width, float height);
	void UpdateCollider();
	bool isColliding(const CIwFVec2& other);
	bool isColliding(Sprite* other, const CIwFVec2& offset);
	bool isOnCamera(Camera* cam);
	void Animate(double dt);

	void Jump();

	void Debug_PrintPos();
	bool ShowColliderPos;

	bool TEMP_JUSTJUMPED;
	bool TEMP_LANDEDJUMP;

protected:

private:
	void SetLastX(float x){ m_LastMovementVal.x = x;}
	void SetLastY(float y){ m_LastMovementVal.y = y;}
	CIwSVec2 m_Center; // The center of the image to rotate around
	CIwFVec2 m_Position; // Screen position
	CIwFVec2 m_LastPosition;
	CIwFVec2 m_LastMovementVal;
	CIwFVec2 m_MovSpeed;
	CIwSVec2 m_Region;
	
	iwangle m_Angle;
	bool m_Animated;
	bool m_hasCollider;
	float m_AnimSpeed;
	float m_CurrentFrame;
	float m_yVel;
	float m_Width, m_Height;
	const char* m_Name;
	float m_JumpTarget_y;
	float posBeforeJump;
	/* 
		The difference between CIWsVec2 and CIwFVec2
		CIwSvec2 takes in a 16bit bit signed integer (int16)
		CIwFVec2 takes in floating point numbers (float)
	*/
	CIwSVec2 m_FrameSize;
	CIwSVec2 m_FrameCount;
	CIw2DImage* m_Image;
	Collider* m_Collider;

	FACING m_facingDir;
	ANIMSTATE m_AnimationState;

};

#endif