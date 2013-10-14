#ifndef SPRITE_H
#define SPRITE_H

class Sprite
{
public:
	Sprite(const char* image);
	~Sprite();
	
	inline void SetCenter(CIwSVec2 center) { m_Center = center; }
	inline CIwFVec2 GetPosition() { return m_Position; }
	inline void SetPosition(CIwFVec2 position) { m_Position = position; }
	inline void SetMovSpeed(CIwFVec2 speed) { m_MovSpeed = speed; }

	void SetMaterial();

	void Testfunc();
	
	inline iwangle GetRotation() { return m_Angle; }
	inline void SetRotation(iwangle angle) { m_Angle = angle; }
	
	void SetAnimated(bool animated, float speed, CIwFVec2 frameCount = CIwFVec2());
	void Update(float deltaTime);
	void Draw();
	bool isColliding(Sprite*);

protected:
	inline float GetHeight();
	inline float GetWidth();

private:
	CIwFVec2 m_Center; // The centre of the image to rotate around
	CIwFVec2 m_Position; // Screen position
	CIwFVec2 m_MovSpeed;
	
	iwangle m_Angle;
	bool m_Animated;
	float m_AnimSpeed;
	float m_CurrentFrame;
	/* 
		The difference between CIWsVec2 and CIwFVec2
		CIwSvec2 takes in a 16bit bit signed integer (int16)
		CIwFVec2 takes in floating point numbers (float)
	*/
	CIwSVec2 m_FrameSize;
	CIwSVec2 m_FrameCount;
	CIw2DImage* m_Image;
};

#endif