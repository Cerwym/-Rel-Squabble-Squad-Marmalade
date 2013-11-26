#include "camera.h"

#include "IwGx.h"
#include <iostream>

Camera::Camera(int w, int h)
{
	m_Position = CIwFVec2(0,0);
	Position = CIwSVec2(0,0);
	screenHeight = h;
	screenWidth = w;
}

Camera::~Camera()
{
	Iw2DSetTransformMatrix(CIwMat2D::g_Identity);
}

void Camera::SetPosition(const CIwSVec2& p)
{
	CIwSVec2 tempP = p;
	if (p.x > 0)
		tempP.x = 0;

	if (p.x < -m_LevelBounds.x - 32 + screenWidth)
		tempP.x = -m_LevelBounds.x - 32 + screenWidth;

	m_Position = tempP;
	CIwMat2D test = CIwMat2D::g_Identity;
	test.SetTrans(m_Position);
	Iw2DSetTransformMatrix(test);
	m_Transform = test;
}

void Camera::MoveBy(const CIwSVec2& m)
{
	m_Position += m;
	SetPosition(m_Position);
}
