#include "GameObject.h"

GameObject::GameObject(const char* name, ObjectType t, bool flag) : Sprite (name, flag)
{
	m_Type = t;
	IsActive = true;
	m_AbilityUsed = false;
	m_targetPosition = CIwFVec2(0,0);
	m_SoundEffect = NULL;
}

GameObject::GameObject(const char* name, ObjectType t, bool flag, CIwFVec2 frames) : Sprite (name, flag, frames)
{
	m_Type = t;
	IsActive = true;
	IsAnimated = true;
	m_AbilityUsed = false;
	m_targetPosition = CIwFVec2(0,0);
	m_SoundEffect = NULL;
}

GameObject::GameObject(const char* name, ObjectType t, bool flag, char* spec) : Sprite (name, flag)
{
	m_Type = t;
	IsActive = true;
	m_AbilityUsed = false;
	m_targetPosition = CIwFVec2(0,0);
	m_SoundEffect = new SoundEffect(spec);
}

GameObject::~GameObject()
{
	if (m_SoundEffect)
		delete m_SoundEffect;
}

void GameObject::DoAbility(const CIwFVec2& target, const double dt)
{
	if (m_Type == Elevator)
	{
		if (target != CIwFVec2(0,0)) // if the elevator is down
		{
			// get the difference between the start position and the target and move the elevator up by that amount / X every gameSecond
			if (GetPosition().y >= target.y)
			{
				TargetReached = false;
				MoveBy(CIwFVec2(0, -3 * dt), 0);
			}
			else
			{
				TargetReached = true;
			}
				
		}
		else
		{
			if (GetPosition().y < m_startPosition.y)
			{
				TargetReached = false;
				MoveBy(CIwFVec2(0, 2 * dt), 0);
			}
			else
			{
				TargetReached = true;
			}
		}
	}
}

void GameObject::PlayEffect()
{
	m_SoundEffect->Play();
}

void GameObject::ResetEffect()
{
	m_SoundEffect->ResetCounter();
}
