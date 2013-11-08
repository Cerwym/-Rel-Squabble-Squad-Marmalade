#include "Collider.h"
#include <iostream>

Collider::Collider(CIwFVec2 pos, float w, float h)
{
	m_Collider = Iw2DCreateImageResource("collisionRect");
	m_size = CIwFVec2(w, h);
	m_Center = CIwSVec2(m_size.x /2.0f, m_size.y / 2.0f);
	m_Position = pos;
}

/*
CIwFVec2 Collider::isColliding(Collider* other)
{
	// declare top left bottom right of rects for collision calculations
	CIwFVec2 TopLeft1 = CIwFVec2(m_Position.x, m_Position.y);
	CIwFVec2 TopLeft2 = CIwFVec2(other->GetPosition().x, other->GetPosition().y);
	CIwFVec2 BottomRight1 = CIwFVec2(m_Position.x + m_size.x, m_Position.y + m_size.y);
	CIwFVec2 BottomRight2 = CIwFVec2(other->GetPosition().x + m_size.x, other->GetPosition().y + m_size.y);

	CIwFVec2 Result = CIwFVec2(0.0f, 0.0f);

	// default as last collision incase there was no change
	//Result = OldCollision;

	// check all sides for a collision
	if(!((TopLeft1.x > BottomRight2.x)||(BottomRight1.x < TopLeft2.x)||(TopLeft1.y > BottomRight2.y)||(BottomRight1.y < TopLeft2.y)))
	{
		//Result = CIwFVec2(0.001f, 0.0f);
		// there is a collision 
		if((m_OldBR.x < TopLeft2.x)&&(BottomRight1.x >= TopLeft2.x))
		{
			// right
			Result = CIwFVec2( BottomRight1.x - m_OldBR.x, 0.0f);
		}

		if((m_OldTL.x >= BottomRight2.x)&&(TopLeft1.x < BottomRight2.x))
		{
			// left
			Result = CIwFVec2(TopLeft1.x - m_OldTL.x, 0.0f);
		}

		if((m_OldBR.y < TopLeft2.y)&&(BottomRight1.y >= TopLeft2.y))
		{
			// bottom
			Result = CIwFVec2(0.0f, BottomRight1.y - m_OldBR.y);
		}

		if((m_OldTL.y >= BottomRight2.y)&&(TopLeft1.y < BottomRight2.y))
		{
			// top
			Result = CIwFVec2(0.0f,TopLeft1.y - m_OldTL.y);
		}

	}
	else
	{
		// update the old bounds if no collision has happened these are "Safe" coordinates
		m_OldTL = CIwFVec2(m_Position.x, m_Position.y);	
		m_OldBR = CIwFVec2(m_Position.x + m_size.x, m_Position.y + m_size.y);
	}

	// return the collision	data
	return Result;
}*/

bool Collider::isColliding(Collider* other)
{
	// declare top left bottom right of rects for collision calculations
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
		//Result = CIwFVec2(0.001f, 0.0f);
		// there is a collision 
		if((m_OldBR.x < TopLeft2.x)&&(BottomRight1.x >= TopLeft2.x))
		{
			// right
			Result = CIwFVec2( BottomRight1.x - m_OldBR.x, 0.0f);
			h = true;
		}

		if((m_OldTL.x >= BottomRight2.x)&&(TopLeft1.x < BottomRight2.x))
		{
			// left
			Result = CIwFVec2(TopLeft1.x - m_OldTL.x, 0.0f);
			h = true;
		}

		if((m_OldBR.y < TopLeft2.y)&&(BottomRight1.y >= TopLeft2.y))
		{
			// bottom
			Result = CIwFVec2(0.0f, BottomRight1.y - m_OldBR.y);
			h = true;
		}

		if((m_OldTL.y >= BottomRight2.y)&&(TopLeft1.y < BottomRight2.y))
		{
			// top
			Result = CIwFVec2(0.0f,TopLeft1.y - m_OldTL.y);
			h = true;
		}

	}
	else
	{
		// update the old bounds if no collision has happened these are "Safe" coordinates
		m_OldTL = CIwFVec2(m_Position.x, m_Position.y);	
		m_OldBR = CIwFVec2(m_Position.x + m_size.x, m_Position.y + m_size.y);
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