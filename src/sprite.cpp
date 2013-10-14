#include <math.h>
#include <IwMaterial.h>
#include "Iw2D.h"
#include "sprite.h"

Sprite::Sprite(const char* image): m_Center(0,0), m_Position(0,0),m_MovSpeed(0,0),
m_Angle(0),m_Animated(false)
{
	m_Image = Iw2DCreateImageResource(image);
}


Sprite::~Sprite()
{
	delete m_Image;
}

inline float Sprite::GetHeight()
{
	return m_Image->GetHeight();
}

inline float Sprite::GetWidth()
{
	return m_Image->GetWidth();
}


void Sprite::Testfunc()
{
}

void Sprite::SetAnimated(bool animated, float speed, CIwFVec2 frameCount)
{
	m_Animated = animated;
	m_AnimSpeed = speed;

	if (animated) 
	{
		m_FrameSize = CIwFVec2((m_Image->GetWidth() / frameCount.x), (m_Image->GetHeight() / frameCount.y));
		m_CurrentFrame = 0;
		m_FrameCount = frameCount; 
	}
}

bool Sprite::isColliding(Sprite* other)
{
	return (abs(m_Position.x - other->GetPosition().x) * 2 < (m_Image->GetWidth() + other->GetWidth()))
		&& (abs(m_Position.y - other->GetPosition().y) * 2 <(m_Image->GetHeight() + m_Image->GetHeight()));
	
	// if a bounding box collision occurs, check for per-pixel collision.
	// possibly use mageneta as the test?
}
void Sprite::Update(float deltaTime)
{
	// update animation
	if (m_Animated)
	{
		m_CurrentFrame += m_AnimSpeed * deltaTime;

		if (m_CurrentFrame > (m_FrameCount.x * m_FrameCount.y)) { m_CurrentFrame = 0; }
	}

	// update position
	m_Position += m_MovSpeed * deltaTime;
}


void Sprite::Draw() 
{
	CIwFVec2 drawPos(m_Position.x, m_Position.y);
	drawPos -=m_Center;

	if (m_Angle != 0)
	{
		CIwFMat2D rotMatrix;
		rotMatrix.SetRot(m_Angle, CIwFVec2((int32)m_Position.x, (int32)m_Position.y));
		Iw2DSetTransformMatrix(rotMatrix);
	}

	if (m_Animated) 
	{
		CIwFVec2 offset(((int)m_CurrentFrame % m_FrameCount.x) * m_FrameSize.x, ((int)m_CurrentFrame / m_FrameCount.x) * m_FrameSize.y);
		Iw2DDrawImageRegion(m_Image, drawPos, offset, m_FrameSize);
	} 
	else
	{
		Iw2DDrawImage(m_Image, drawPos);
	}

	if (m_Angle != 0)
	{
		Iw2DSetTransformMatrix(CIwFMat2D::g_Identity);
	}
}