#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include "sprite.h"

enum ObjectType {Elevator, Button, Door, Terminal, Scenerary, Exit};

class GameObject : public Sprite
{
public:
	GameObject(const char* name, ObjectType t);

	void AddChildObject(GameObject* obj) {m_Child = obj;}
	void AddTag(std::string tag) {m_Tag = tag;}

	GameObject* Child(){return m_Child;}
	ObjectType GetType(){return m_Type;}

	bool IsActive;
private:
	ObjectType m_Type;
	
	// Pointer to the child object, so one object can be 'attached' to another
	GameObject* m_Child;
	std::string m_Tag;
};

#endif