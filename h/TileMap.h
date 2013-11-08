#ifndef TILEMAP_H_
#define TILEMAP_H_

#include <vector>
#include "GameObject.h"

class TileMap
{
public:
	TileMap (const char* lvlFile);

	void Update();
	void Draw();
	std::vector<GameObject*> GetMap(){return m_Map;}
	std::vector<GameObject*> GetObjects(){return m_Objects;}

private:
	void AddRelationships();
	
	std::vector<GameObject*> m_Map;
	std::vector<GameObject*> m_Objects;
};
#endif