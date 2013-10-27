#include "sprite.h"
#include "camera.h"
#include <iostream>

Sprite::Sprite(const char* image): m_Center(0,0), m_Position(0,0),m_MovSpeed(0,0),
m_Angle(0),m_Animated(false)
{
	m_Image = Iw2DCreateImageResource(image);
	m_yVel = 0;
	TEMP_ISFALLING = false;
	TEMP_JUSTJUMPED = false;
	TEMP_LANDEDJUMP = true;
}


Sprite::~Sprite()
{
	delete m_Image;
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

bool Sprite::isColliding(Sprite* const other)
{
	return (abs(m_Position.x - other->GetPosition().x) * 2 < (m_Image->GetWidth() + other->GetWidth()))
		&& (abs(m_Position.y - other->GetPosition().y) * 2 <(m_Image->GetHeight() + other->GetHeight()));
	
	// if a bounding box collision occurs, check for per-pixel collision.
	// possibly use magenta as the test?
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

	// Update gravity
	if ((m_Position.y < 320) && TEMP_ISFALLING == true) // and has jumped
	{
		m_yVel += GRAVITY;
		m_Position.y += m_yVel;
		if (m_Position.y >= 320)
		{
			std::cout << "I'm supposed to have stopped" << std::endl;
			TEMP_ISFALLING = false;
			m_yVel = 0;
		}
		
		std::cout << "I'm Falling @ " << m_yVel << " mp/s" << std::endl;
	}

	if (TEMP_JUSTJUMPED == true && TEMP_LANDEDJUMP == true)
	{
		TEMP_BEFOREJUMPY = m_Position.y;
		TEMP_JUSTJUMPED = false;
		TEMP_LANDEDJUMP = false;
	}

	if (TEMP_JUSTJUMPED == true && TEMP_LANDEDJUMP == false)
	{
		m_yVel += GRAVITY;
		m_Position.y -= m_yVel;
		std::cout << "Still rising" << std::endl;
		std::cout << "Pos is " << m_Position.y << std::endl;
		if (m_Position.y <= (TEMP_BEFOREJUMPY - JUMP_HEIGHT)) // TEMP BEFORE Y IS THE PROBLEM.
		{
			m_yVel = 0;
			//TEMP_BEFOREJUMPY = m_Position.y;
			TEMP_ISFALLING = true;
			TEMP_LANDEDJUMP = true;
			std::cout << "landed" << std::endl;
		}
	}

	//std::cout << "Velocity is " << m_yVel << std::endl;
}

void Sprite::Draw() 
{
	CIwSVec2 drawPos(m_Position.x , m_Position.y);
	drawPos -=m_Center;

	if (m_Angle != 0)
	{
		CIwMat2D rotMatrix;
		rotMatrix.SetRot(m_Angle, CIwVec2((int32)m_Position.x, (int32)m_Position.y));
		//Iw2DSetTransformMatrix(rotMatrix);
	}

	// scale // rotate //translate
	if (m_Animated) 
	{
		CIwSVec2 offset(((int)m_CurrentFrame % m_FrameCount.x) * m_FrameSize.x, ((int)m_CurrentFrame / m_FrameCount.x) * m_FrameSize.y);
		Iw2DDrawImageRegion(m_Image, drawPos, offset, m_FrameSize);
	} 
	else
	{
		Iw2DDrawImage(m_Image, drawPos);
	}

	if (m_Angle != 0)
	{
		//Iw2DSetTransformMatrix(CIwMat2D::g_Identity);
	}
}

void Sprite::Debug_PrintPos()
{
	std::cout << "Pos x : " << m_Position.x << " y : " << m_Position.y << std::endl;
}