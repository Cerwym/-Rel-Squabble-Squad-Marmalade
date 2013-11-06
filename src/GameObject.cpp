#include "GameObject.h"

GameObject::GameObject(const char* name, ObjectType t) : Sprite (name)
{
	m_Type = t;
}