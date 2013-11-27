#include "sprite.h"
#include <iostream>
#include <assert.h>

Sprite::Sprite(const char* name, bool flag): m_Position(0,0),m_MovSpeed(0,0),m_Angle(0),m_Animated(false)
{
	// hashed name for the sprite in IwResourceManager
	m_Image = Iw2DCreateImageResource(name);
	m_Width = m_Image->GetWidth();
	m_Height = m_Image->GetHeight();
	m_yVel = 0;
	m_Name = name;

	m_Center = CIwSVec2(static_cast<int16>(m_Width) / 2, static_cast<int16>(m_Height) / 2);
	TEMP_JUSTJUMPED = false;
	TEMP_LANDEDJUMP = true;
	TEMP_ISCOLLIDING = false;
	ShowColliderPos = false;
	m_hasCollider = flag;
	if (flag){ m_Collider = new Collider(m_Position, m_Width, m_Height);}
	m_facingDir = FACING_RIGHT;
}

Sprite::Sprite(const char* name, bool flag, CIwFVec2 frameCount): m_Position(0,0),m_MovSpeed(0,0),m_Angle(0),m_Animated(true)
{
	// hashed name for the sprite in IwResourceManager
	m_Image = Iw2DCreateImageResource(name);
	m_yVel = 0;
	m_Name = name;

	TEMP_JUSTJUMPED = false;
	TEMP_LANDEDJUMP = true;
	TEMP_ISCOLLIDING = false;
	ShowColliderPos = false;
	m_hasCollider = flag;
	m_facingDir = FACING_RIGHT;

	// 1/2 frame every game second
	SetAnimated(true, 0.5, frameCount);
}


Sprite::~Sprite()
{
	delete m_Image;
	//delete m_Collider;
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
		int16 fw = m_Image->GetWidth() / frameCount.x;
		int16 fh = m_Image->GetHeight() / frameCount.y;
		m_FrameSize = CIwSVec2(fw, fh); // this is the bug
		m_CurrentFrame = 0;
		m_FrameCount.x = frameCount.x;
		m_FrameCount.y = frameCount.y;
	}

	m_Width = m_FrameSize.x;
	m_Height = m_FrameSize.y;

	m_Center = CIwSVec2(static_cast<int16>(m_Width) / 2, static_cast<int16>(m_Height) / 2);

	if (m_hasCollider){m_Collider = new Collider(m_Position, m_Width, m_Height);}
}

// Collision detection for bounding box, commonly used to detect if a player has clicked on a UI element or character.
bool Sprite::isColliding(const CIwFVec2& other)
{
	IW_CALLSTACK("Sprite::isColliding");

	// convert to local coordinate space
	CIwSVec2 localPos = CIwSVec2
		(static_cast<int16>(other.x), static_cast<int16>(other.y)) -
		CIwSVec2(static_cast<int16>(m_Position.x), static_cast<int16>(m_Position.y));

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

	if (m_hasCollider && other->hasCollider())
		return(m_Collider->isColliding(other->m_Collider));
	else
		return false;
}

void Sprite::MoveBy(const CIwFVec2& val, double dt) 
{
	m_LastPosition = m_Position;
	m_Position += CIwFVec2(static_cast<float>(val.x), static_cast<float>((val.y)));
	if (m_Animated)
	{
		Animate(dt);
	}

	if (val.x != 0) m_LastMovementVal.x = val.x;
	if (val.y != 0) m_LastMovementVal.y = val.y;

	// Play walking sound
	//Playwalkingsound() Disabled because it constantly fires
}

void Sprite::Animate(double dt)
{
	m_CurrentFrame += m_AnimSpeed * dt;
	if (m_CurrentFrame > (m_FrameCount.x * m_FrameCount.y))
		m_CurrentFrame = 0;
}

void Sprite::Update(float deltaTime)
{
	// Make the character fall each frame
	MoveBy(CIwFVec2(0, 2), 0);

	if (TEMP_JUSTJUMPED) // If the jump button has been process, enter this loop until the height of the jump has been reached
	{
		if (TEMP_LANDEDJUMP == false)
		{
			if(GetPosition().y >= m_JumpTarget_y)
				MoveBy(CIwFVec2(0, -8), 0);
		}

		if (GetPosition().y <= m_JumpTarget_y)
		{
			TEMP_JUSTJUMPED = false;
		}

	}
	if (TEMP_JUSTJUMPED == false && TEMP_LANDEDJUMP == false)
	{
		if (GetPosition().y >= posBeforeJump)
		{
			TEMP_LANDEDJUMP = true;
		}
	}

	UpdateCollider();
}

void Sprite::UpdateCollider()
{
	if (m_hasCollider)
		m_Collider->Update(m_Position);
}

void Sprite::Jump()
{
	if (TEMP_LANDEDJUMP == false && m_Position.y <= m_JumpTarget_y && TEMP_JUSTJUMPED == false)
	{
		TEMP_LANDEDJUMP = true;
	}

	if (TEMP_LANDEDJUMP == true)
	{
		posBeforeJump = GetPosition().y;
		TEMP_JUSTJUMPED = true;
		m_JumpTarget_y = m_Position.y - JUMP_HEIGHT;
		std::cout << "Jump height has been set to " << m_JumpTarget_y << std::endl;
		TEMP_LANDEDJUMP = false;
	}
}

void Sprite::Draw() 
{
	CIwSVec2 drawPos((int16)m_Position.x, (int16)m_Position.y);

	if (m_facingDir == FACING_RIGHT)
		Iw2DSetImageTransform(IW_2D_IMAGE_TRANSFORM_NONE);
	else if (m_facingDir == FACING_LEFT)
		Iw2DSetImageTransform(IW_2D_IMAGE_TRANSFORM_FLIP_X);

	if (m_Animated) 
	{
		CIwSVec2 offset(((int)m_CurrentFrame % m_FrameCount.x) * m_FrameSize.x, ((int)m_CurrentFrame / m_FrameCount.x) * m_FrameSize.y);
		Iw2DDrawImageRegion(m_Image, drawPos, offset, m_FrameSize);
		if (ShowColliderPos)
			if (m_hasCollider)
				m_Collider->Draw();
	} 
	else
	{
		Iw2DDrawImage(m_Image, drawPos);
		if (ShowColliderPos)
			if (m_hasCollider)
				m_Collider->Draw();
	}
}

// Give the camera's position to keep the sprites on screen
void Sprite::Draw(CIwSVec2& camPos) 
{
	if (m_facingDir == FACING_RIGHT)
		Iw2DSetImageTransform(IW_2D_IMAGE_TRANSFORM_NONE);
	else if (m_facingDir == FACING_LEFT)
		Iw2DSetImageTransform(IW_2D_IMAGE_TRANSFORM_FLIP_X);

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
			if (m_hasCollider)
				m_Collider->Draw();
	}
}

void Sprite::Debug_PrintPos()
{
	std::cout << "Pos x : " << m_Position.x << " y : " << m_Position.y << std::endl;
	if (ShowColliderPos)
	{
		if (m_hasCollider)
			m_Collider->DEBUG_PRINTPOS();
	}
}
