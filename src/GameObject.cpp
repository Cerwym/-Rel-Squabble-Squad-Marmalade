#include "GameObject.h"

GameObject::GameObject(const char* name, ObjectType t, bool flag) : Sprite (name, flag)
{
	m_Type = t;
	IsActive = true;
}

GameObject::GameObject(const char* name, ObjectType t, bool flag, CIwFVec2 frames) : Sprite (name, flag, frames)
{
	m_Type = t;
	IsActive = true;
	IsAnimated = true;
}