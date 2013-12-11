#include "Collider.h"
#include <iostream>

Collider::Collider(CIwFVec2 pos, float w, float h)
{
	m_Image = Iw2DCreateImageResource("collisionRect");
	m_size = CIwFVec2(w, h);
	m_Center = CIwSVec2(m_size.x /2.0f, m_size.y / 2.0f);
	m_Position = pos;

}

Collider::~Collider()
{
	delete m_Image;
}

bool Collider::isColliding(Collider* other, const CIwFVec2& offset)
{
	// declare top left bottom right of rects for collision calculations
	CIwFVec2 TopLeft1 = CIwFVec2(m_Position.x, m_Position.y);
	CIwFVec2 TopLeft2 = CIwFVec2(other->GetPosition().x, other->GetPosition().y) + offset;
	CIwFVec2 BottomRight1 = CIwFVec2(m_Position.x + m_size.x, m_Position.y + m_size.y);
	CIwFVec2 BottomRight2 = CIwFVec2(other->GetPosition().x + other->m_size.x, other->GetPosition().y + other->m_size.y) + offset;

	CIwFVec2 Result = CIwFVec2(0.0f, 0.0f);

	// default as last collision in case there was no change
	//Result = OldCollision;
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

void Collider::Update(CIwFVec2 pos)
{
	m_Position = pos;
	m_DrawPos = CIwSVec2(m_Position.x, m_Position.y);

	//m_DrawPos -= CIwSVec2(m_Center.x, m_Center.y);
	SetPosition(CIwFVec2(m_DrawPos.x, m_DrawPos.y));
}

void Collider::Draw()
{
	Iw2DDrawImage(m_Image, m_DrawPos, CIwSVec2(m_size.x, m_size.y));
}

void Collider::DEBUG_PRINTPOS()
{
	std::cout << "Collider Pos -> " << m_Position.x << "," << m_Position.y << std::endl;
}