#include "Collider.h"
#include <iostream>

Collider::Collider(CIwFVec2 pos, float w, float h)
{
	m_Collider = Iw2DCreateImageResource("collisionRect");
	m_size = CIwFVec2(w, h);
	m_Center = CIwSVec2(m_size.x /2.0f, m_size.y / 2.0f);
	m_Position = pos;
}

Collider::~Collider()
{
	delete m_Collider;
}

bool Collider::isColliding(Collider* other)
{
	// declare top left bottom right of rects for collision calculations
	CIwFVec2 TopLeft1 = CIwFVec2(m_Position.x, m_Position.y);
	CIwFVec2 TopLeft2 = CIwFVec2(other->GetPosition().x, other->GetPosition().y);
	CIwFVec2 BottomRight1 = CIwFVec2(m_Position.x + m_size.x, m_Position.y + m_size.y);
	CIwFVec2 BottomRight2 = CIwFVec2(other->GetPosition().x + other->m_size.x, other->GetPosition().y + other->m_size.y);

	// default as last collision in case there was no change
	bool h = false;
	// check all sides for a collision
	if(!((TopLeft1.x > BottomRight2.x)||(BottomRight1.x < TopLeft2.x)||(TopLeft1.y > BottomRight2.y)||(BottomRight1.y < TopLeft2.y)))
	{
		if((m_OldBR.x < TopLeft2.x)&&(BottomRight1.x >= TopLeft2.x))
			m_CollisionLocation.Right = true;	// right
		//else
			//m_CollisionLocation.Right = false;

		if((m_OldTL.x >= BottomRight2.x)&&(TopLeft1.x < BottomRight2.x))
			m_CollisionLocation.Left = true;
		//else
			//m_CollisionLocation.Left = false;

		if((m_OldBR.y < TopLeft2.y)&&(BottomRight1.y >= TopLeft2.y))
			m_CollisionLocation.Bottom = true;
		//else
			//m_CollisionLocation.Bottom = false;  

		if((m_OldTL.y >= BottomRight2.y)&&(TopLeft1.y < BottomRight2.y))
			m_CollisionLocation.Top = true;
		//else
			//m_CollisionLocation.Top = false;

		// Top Left = TopLeft1 - 
		h = true;
	}
	else
	{
		// update the old bounds if no collision has happened these are "Safe" coordinates
		m_OldTL = CIwFVec2(m_Position.x, m_Position.y);	
		m_OldBR = CIwFVec2(m_Position.x + m_size.x, m_Position.y + m_size.y);

		m_CollisionLocation.Left = false;
		m_CollisionLocation.Right = false;
		m_CollisionLocation.Bottom = false;
		m_CollisionLocation.Top = false;
		h = false;
	}

	// return the collision	data
	return h;
}

void Collider::Update(CIwFVec2 pos)
{
	m_Position = pos;
	m_DrawPos = CIwSVec2(m_Position.x, m_Position.y);

	SetPosition(CIwFVec2(m_DrawPos.x, m_DrawPos.y));
}

void Collider::Draw()
{
	Iw2DDrawImage(m_Collider, m_DrawPos, CIwSVec2(m_size.x, m_size.y));
}

void Collider::DEBUG_PRINTPOS()
{
	std::cout << "Collider Pos -> " << m_Position.x << "," << m_Position.y << std::endl;
}