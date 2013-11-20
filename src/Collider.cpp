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
		h = true;
	else
	{
		m_OldTL = CIwFVec2(m_Position.x, m_Position.y);        
		m_OldBR = CIwFVec2(m_Position.x + m_size.x, m_Position.y + m_size.y);
		h = false;
	}

	// return the collision	data
	return h;
}

CIwFVec2 Collider::isCollidingC(Collider* other) // add in force it's moving by
{
	// declare top left bottom right of rects for collision calculations
	//std::cout << "Old TL" << m_OldTL.x << "," << m_OldTL.y << std::endl;
	//m_OldTL = CIwFVec2(m_Position.x, m_Position.y);        
	//m_OldBR = CIwFVec2(m_Position.x + m_size.x, m_Position.y + m_size.y);

	CIwFVec2 TopLeft1 = CIwFVec2(m_Position.x, m_Position.y);
	CIwFVec2 TopLeft2 = CIwFVec2(other->GetPosition().x, other->GetPosition().y);
	CIwFVec2 BottomRight1 = CIwFVec2(m_Position.x + m_size.x, m_Position.y + m_size.y);
	CIwFVec2 BottomRight2 = CIwFVec2(other->GetPosition().x + other->m_size.x, other->GetPosition().y + other->m_size.y);

	CIwFVec2 Result = CIwFVec2(0.0f, 0.0f);

	// default as last collision incase there was no change
	//Result = OldCollision;
	bool h = false;
	// check all sides for a collision
	if(!((TopLeft1.x > BottomRight2.x)||(BottomRight1.x < TopLeft2.x)||(TopLeft1.y > BottomRight2.y)||(BottomRight1.y < TopLeft2.y)))
	{

		if(m_OldTL.x < TopLeft2.x && TopLeft2.x < m_OldBR.x && m_OldBR.x < BottomRight2.x){   
			Result.x += (TopLeft1.x - m_OldTL.x - 0.5);
			m_CollisionLocation.Left = true;
			h = true;
		}

		if(TopLeft2.x < m_OldTL.x && m_OldTL.x < BottomRight2.x && BottomRight2.x < m_OldBR.x){
			// right
			//Result = CIwFVec2( BottomRight1.x - m_OldBR.x, 0.0f);
			Result.x += (BottomRight1.x - m_OldBR.x);
			m_CollisionLocation.Right = true;
			h = true;
		}

		if(m_OldTL.y < TopLeft2.y && TopLeft2.y < m_OldBR.y && m_OldBR.y < BottomRight2.y){
			// top
			//Result = CIwFVec2(0.0f,TopLeft1.y - m_OldTL.y);
			Result.y += (TopLeft1.y - m_OldTL.y);
			m_CollisionLocation.Bottom = true;
			h = true;
		}else
		{
			m_CollisionLocation.Bottom = false;
		}

		if(TopLeft2.y < m_OldTL.y && m_OldTL.y < BottomRight2.y && BottomRight2.y < m_OldBR.y){
			// bottom
			//Result = CIwFVec2(0.0f, BottomRight1.y - m_OldBR.y);
			Result.y += (BottomRight1.y - m_OldBR.y);
			m_CollisionLocation.Top = true;
			h = true;
		}else
		{
			m_CollisionLocation.Top = false;
		}
	}
	else
	{
		// update the old bounds if no collision has happened these are "Safe" coordinates
		m_OldTL = CIwFVec2(m_Position.x, m_Position.y);        
		m_OldBR = CIwFVec2(m_Position.x + m_size.x, m_Position.y + m_size.y);
		m_CollisionLocation.Top = false;
		m_CollisionLocation.Bottom = false;
		m_CollisionLocation.Left = false;
		m_CollisionLocation.Right = false;
		h = false;
	}

	// return the collision        data
	return Result;
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