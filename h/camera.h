#ifndef CAMERA_H_
#define CAMERA_H_

#include "Iw2D.h"

class Camera
{
public:

	Camera();
	~Camera();
	
	CIwSVec2 GetPosition(){return m_Position;}
	void SetPosition(const CIwSVec2& p)
	{
		m_Position = p;
		CIwMat2D test = CIwMat2D::g_Identity;
		test.SetTrans(m_Position);
		Iw2DSetTransformMatrix(test);
		m_Transform = test;
	}

	void MoveBy(const CIwSVec2& m)
	{
		m_Position += m;
		SetPosition(m_Position);
	}

	inline CIwMat2D GetTranslation(){return m_Transform;}
	
	void Debug_PrintPosition();

	void Update();
	CIwSVec2 Position;
private:

	// The position of the camera in relation to the world view
	CIwSVec2 m_Position;
	CIwFVec2 viewPort;
	float camSpeed;
	CIwMat2D m_Transform;

};

#endif