#ifndef CAMERA_H_
#define CAMERA_H_

#include "s3e.h"
#include "IwGx.h"
#include "IwGeom.h"

class Camera
{
public:
	Camera() : Position(0,0), Scale(1.0f), Angle(0){}
	virtual ~Camera() {}

	CIwMat2D&	getTransform() {return Transform;}
	void		setPosition(const float x, const float y) {Position.x = x; Position.y = y;}
	CIwFVec2	getPosition() const {return Position;}
	void		setScale(const float scale) {Scale = scale;}
	float		getScale() const {return Scale;}
	void		setAngle(const float angle) {Angle = angle;}
	float		getAngle() const {return Angle;}

	virtual void Update();

protected:
	CIwMat2D Transform;
	CIwFVec2 Position;
	float Scale;
	float Angle;
	// Maybe rebuild transform
};

#endif