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

	bool isColliding(Collider* other, const CIwFVec2& offset);

	void DEBUG_PRINTPOS();

private:

	CIwSVec2 m_Center;
	CIwFVec2 m_Position;
	CIwSVec2 m_DrawPos;

	CIwFVec2 m_size;
	
	CIw2DImage* m_Collider;

	CIwFVec2 m_OldTL;
	CIwFVec2 m_OldBR;
};

#endif