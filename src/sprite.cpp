#include "sprite.h"
#include <iostream>
#include <assert.h>

Sprite::Sprite(const char* name): m_Position(0,0),m_MovSpeed(0,0),
m_Angle(0),m_Animated(false)
{
	// hashed name for the sprite in IwResourceManager
	m_Image = Iw2DCreateImageResource(name);
	m_Width = m_Image->GetWidth();
	m_Height = m_Image->GetHeight();
	m_yVel = 0;
	m_Name = name;

	m_Center = CIwSVec2(static_cast<int16>(m_Width) / 2, static_cast<int16>(m_Height) / 2);
	TEMP_ISFALLING = true;
	TEMP_JUSTJUMPED = false;
	TEMP_LANDEDJUMP = true;
	TEMP_ISCOLLIDING = false;
	ShowColliderPos = false;
	m_Collider = new Collider(m_Position, m_Width, m_Height);
}


Sprite::~Sprite()
{
	delete m_Image;
	delete m_Collider;
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

//Simple bounding box detection, no left right top or bottom.
bool Sprite::isColliding(const CIwFVec2& other)
{
	IW_CALLSTACK("Sprite::isColliding");

	// convert to local coordinate space
	CIwSVec2 localPos = CIwSVec2
		(static_cast<int16>(other.x), static_cast<int16>(other.y)) -
		CIwSVec2(static_cast<int16>(m_Position.x), static_cast<int16>(m_Position.y));

	//std::cout << "localPos->" << localPos.x << "," << localPos.y << std::endl; 

	if(localPos.x < 0
		|| localPos.y < 0
		|| localPos.x > (int32)m_Width
		|| localPos.y > (int32)m_Height)
	{
		return false;
	}


	return true;
}

bool Sprite::isColliding(Sprite* other)
{
	IW_CALLSTACK("Sprite::isColliding");

	return(m_Collider->isColliding(other->m_Collider));
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
		
	//m_Position += m_MovSpeed * deltaTime;

	// Update gravity
	if ((TEMP_ISCOLLIDING == false)) // and has jumped
	{
		m_yVel += GRAVITY;
		m_Position.y += (m_yVel);// / 8);
		//std::cout << "Down Velocity" << std::endl;
		
		//std::cout << "I'm Falling @ " << m_yVel << " mp/s" << std::endl;
	}
	else
	{
		if (!TEMP_JUSTJUMPED)
			m_yVel = 0;
	}

	if (TEMP_JUSTJUMPED == true && TEMP_LANDEDJUMP == true)
	{
		TEMP_BEFOREJUMPY = m_Position.y;
		TEMP_LANDEDJUMP = false;
	}

	if (TEMP_JUSTJUMPED == true)
	{
		m_yVel += GRAVITY;
		m_Position.y += (-m_yVel * 2);
		std::cout << "Velocity " << m_yVel << std::endl;
		if (m_Position.y <= (TEMP_BEFOREJUMPY - 128)) // TEMP BEFORE Y IS THE PROBLEM.
		{
			m_yVel = 0;
			TEMP_JUSTJUMPED = false;
			TEMP_ISFALLING = true;
			TEMP_LANDEDJUMP = true;
			TEMP_ISCOLLIDING = false;
			std::cout << "Finished Jumping" << std::endl;
		}
	}
	UpdateCollider();
}

void Sprite::UpdateCollider()
{
	m_Collider->Update(m_Position);
}

void Sprite::Draw() 
{
	CIwSVec2 drawPos((int16)m_Position.x, (int16)m_Position.y);
	//drawPos -=m_Center;

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
		if (ShowColliderPos)
		{
			m_Collider->Draw();
		}
	}

	if (m_Angle != 0)
	{
		//Iw2DSetTransformMatrix(CIwMat2D::g_Identity);
	}
}

// Give the camera's position to keep the sprites on screen
void Sprite::Draw(CIwSVec2& camPos) 
{
	CIwSVec2 drawPos((int16)m_Position.x - camPos.x , (int16)m_Position.y - camPos.y);
	//drawPos -=m_Center;

	if (m_Animated) 
	{
		CIwSVec2 offset(((int)m_CurrentFrame % m_FrameCount.x) * m_FrameSize.x, ((int)m_CurrentFrame / m_FrameCount.x) * m_FrameSize.y);
		Iw2DDrawImageRegion(m_Image, drawPos, offset, m_FrameSize);
	} 
	else
	{
		Iw2DDrawImage(m_Image, drawPos);
		if (ShowColliderPos)
		{
			m_Collider->Draw();
		}
	}
}

void Sprite::Debug_PrintPos()
{
	std::cout << "Pos x : " << m_Position.x << " y : " << m_Position.y << std::endl;
	if (ShowColliderPos)
		m_Collider->DEBUG_PRINTPOS();
}