#include "sprite.h"
#include "camera.h"
#include <iostream>
#include <assert.h>

Sprite::Sprite(const char* name): m_Center(0,0), m_Position(0,0),m_MovSpeed(0,0),
m_Angle(0),m_Animated(false)
{
	// hashed name for the sprite in IwResourceManager
	m_Image = Iw2DCreateImageResource(name);

	m_Width = m_Image->GetWidth();
	m_Height = m_Image->GetWidth();
	m_yVel = 0;
	TEMP_ISFALLING = true;
	TEMP_JUSTJUMPED = false;
	TEMP_LANDEDJUMP = true;
}


Sprite::~Sprite()
{
	delete m_Image;
}

void Sprite::BuildCollision(const char* fname)
{
	CIwImage img;
	img.LoadFromFile(fname);
	// Build the collision data from the image's alpha channel in 8bit format.
	m_CollisionMap.SetFormat(CIwImage::A_8);
	m_CollisionMap.SetWidth(img.GetWidth());
	m_CollisionMap.SetHeight(img.GetHeight());
	img.ConvertToImage(&m_CollisionMap);
}

// 
void Sprite::BuildCollision(CIwImage img)
{
	m_CollisionMap = img;
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

// Pass in the location of the element to collide against. A click event or Sprite->GetPosition
bool Sprite::isColliding(const CIwFVec2& other)
{
	IW_CALLSTACK("Sprite::isColliding");

	CIwSVec2 localPos = CIwSVec2(static_cast<int>(other.x), static_cast<int>(other.y)) - CIwSVec2(static_cast<int>(m_Position.x), static_cast<int>(m_Position.y));

	if(localPos.x < 0
		|| localPos.y < 0
		|| localPos.x > (int32)m_CollisionMap.GetWidth()
		|| localPos.y > (int32)m_CollisionMap.GetHeight())
	{
		TEMP_ISCOLLIDING = false;
		return false;
	}

	bool t = m_CollisionMap.GetTexels()[localPos.y * m_CollisionMap.GetWidth() + localPos.x] > 0x80; // return true if the alpha value is greater than half
	TEMP_ISCOLLIDING = t;
	return t;
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
	if ((!TEMP_ISCOLLIDING) && TEMP_ISFALLING == true) // and has jumped
	{
		m_yVel += GRAVITY;
		m_Position.y += m_yVel;
		if (TEMP_ISCOLLIDING)
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

// Give the camera's position to keep the sprites on screen
void Sprite::Draw(CIwSVec2& camPos) 
{
	CIwSVec2 drawPos(m_Position.x - camPos.x , m_Position.y - camPos.y);
	drawPos -=m_Center;

	if (m_Animated) 
	{
		CIwSVec2 offset(((int)m_CurrentFrame % m_FrameCount.x) * m_FrameSize.x, ((int)m_CurrentFrame / m_FrameCount.x) * m_FrameSize.y);
		Iw2DDrawImageRegion(m_Image, drawPos, offset, m_FrameSize);
	} 
	else
	{
		Iw2DDrawImage(m_Image, drawPos);
	}
}

void Sprite::Debug_PrintPos()
{
	std::cout << "Pos x : " << m_Position.x << " y : " << m_Position.y << std::endl;
}