#ifndef TILEMAP_H_
#define TILEMAP_H_

#include <vector>
#include "GameObject.h"
#include "camera.h"

class TileMap
{
public:
	TileMap (const char* lvlFile, const char* rFile);
	~TileMap();

	void Update();
	void Draw(Camera* cam, double dt);
	std::vector<GameObject*> GetMap(){return m_Map;}
	std::vector<GameObject*> GetObjects(){return m_Objects;}
	std::vector<CIwFVec2> GetSpawnPositions(){return m_SpawnPos;}

private:
	void AddRelationships(const char* rFile);
	
	std::vector<GameObject*> m_Map;
	std::vector<GameObject*> m_Objects;
	std::vector<CIwFVec2> m_SpawnPos;
};
#endif