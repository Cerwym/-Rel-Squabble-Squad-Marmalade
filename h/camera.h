#ifndef CAMERA_H_
#define CAMERA_H_

#include "Iw2D.h"

class Camera
{
public:

	Camera(int,int);
	~Camera();
	
	CIwSVec2 GetPosition(){return m_Position;}
	void SetPosition(const CIwSVec2& p);

	void Camera::MoveBy(const CIwSVec2& m);

	void SetLevelBounds(CIwFVec2& b) {m_LevelBounds = b;}

	inline CIwMat2D GetTranslation(){return m_Transform;}

	CIwSVec2 Position;
private:

	// The position of the camera in relation to the world view
	CIwSVec2 m_Position;
	CIwFVec2 m_LevelBounds;
	float camSpeed;
	CIwMat2D m_Transform;
	int screenWidth, screenHeight;

};

#endif