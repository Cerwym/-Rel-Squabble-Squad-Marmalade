#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "s3e.h"
#include "IwGx.h"
#include "IwGeom.h"
#include "sprite.h"
#include "spritemanager.h"

//class Camera;

class GameScene
{
public:
	GameScene() : SprManager(NULL){}
	typedef CIwList<Sprite*>::iterator Iterator;
	Iterator begin() {return Actors.begin();}
	Iterator end() {return Actors.end();}


	virtual void Update(const float dt);

	virtual void Draw();

protected:
	CIwList<Sprite*> Actors;
	SpriteManager* SprManager;
	//Camera Cam;
};

#endif