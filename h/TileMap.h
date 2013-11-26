#ifndef TILEMAP_H_
#define TILEMAP_H_

#include <vector>
#include "GameObject.h"

class TileMap
{
public:
	TileMap (const char* lvlFile, const char* rFile);
	~TileMap();

	void Update();
	void Draw(double dt);
	std::vector<GameObject*> GetMap(){return m_Map;}
	std::vector<GameObject*> GetObjects(){return m_Objects;}
	CIwFVec2 GetLevelBounds(){return m_LevelMaxBounds;}
	std::vector<CIwFVec2> GetSpawnPositions(){return m_SpawnPos;}

private:
	void AddRelationships(const char* rFile);
	
	std::vector<GameObject*> m_Map;
	std::vector<GameObject*> m_Objects;
	std::vector<CIwFVec2> m_SpawnPos;

	CIwFVec2 m_LevelMaxBounds;
};
#endif