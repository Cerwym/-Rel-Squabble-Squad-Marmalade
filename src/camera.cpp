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

}

void Camera::Update()
{
	//Iw2DSetTransformMatrix(CIwMat2D::g_Identity);
	//	Iw2DSetTransformMatrix(m_Transform);

	/*
	//stub, maybe use to rebuild transforms
	
	//keep a track of the function callstack at runtime
	IW_CALLSTACK("Camera::Update");

	IwAssertMsg(APP, m_CamTarget, ("Update was called, but the camera has no target"));

	CIwFMat viewMatrix = CIwFMat::g_Identity;

	if (m_CamTarget)
	{
		//
		m_Position.x = m_CamTarget->GetPosition().x;
		m_Position.y = m_CamTarget->GetPosition().y + (m_CamTarget->GetHeight() / 2);

		CIwFVec3 lookPos = m_Position;
		lookPos.y = 0.0f;

		viewMatrix.LookAt(m_Position, lookPos, CIwFVec3::g_AxisZ);
		viewMatrix.t = m_Position;
		std::cout << "Tracking @ " << m_Position.x << "," << m_Position.y << std::endl; 
		std::cout << "vMatrix @: " << viewMatrix.GetTrans().x << "," << viewMatrix.GetTrans().y << "," << viewMatrix.GetTrans().z << std::endl;
	}

	IwGxSetPerspMul((float) IwGxGetScreenWidth() * 0.5f);
	IwGxSetFarZNearZ(6000.0f, 10.0f);
	IwGxSetViewMatrix(&viewMatrix);

	m_VMatrix = viewMatrix;

	std::cout << "Update Finished" << std::endl;

	*/
}

void Camera::Debug_PrintPosition()
{
	std::cout << "Camera's Position -> " << m_Position.x << "," << m_Position.y;
}