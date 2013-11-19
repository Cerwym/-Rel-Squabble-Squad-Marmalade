#ifndef COLLIDER_H
#define COLLIDER_H

#include <math.h>
#include "Iw2D.h"

class Collider
{
public:
	Collider(CIwFVec2 pos, float w, float h);
	~Collider();

	CIwFVec2 GetPosition() {return m_Position;}
	void SetPosition(CIwFVec2 pos) {m_Position = pos;}

	void Update(CIwFVec2 nPos);
	void Draw();

	bool isColliding(Collider* other);
	CIwFVec2 isCollidingC(Collider* other);
	bool CollidingOnStep(Collider* other, CIwFVec2& step);

	// Polish this up
	bool GetTop(){return m_CollisionLocation.Top;}
	bool GetBottom(){return m_CollisionLocation.Bottom;}
	bool GetLeft(){return m_CollisionLocation.Left;}
	bool GetRight(){return m_CollisionLocation.Right;}

	void DEBUG_PRINTPOS();

private:

	CIwSVec2 m_Center;
	CIwFVec2 m_Position;
	CIwSVec2 m_DrawPos;

	CIwFVec2 m_size;
	
	CIw2DImage* m_Collider;

	CIwFVec2 m_OldTL;
	CIwFVec2 m_OldBR;

	struct CollLoc{ bool Top; bool Bottom; bool Left;bool Right;
	}; CollLoc m_CollisionLocation;
};

#endif