#include "camera.h"

#include "IwGx.h"
#include <iostream>

Camera::Camera()
{
	m_Position = CIwFVec2(0,0);
	Position = CIwSVec2(0,0);
}

Camera::~Camera()
{
	Iw2DSetTransformMatrix(CIwMat2D::g_Identity);
}
