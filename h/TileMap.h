#ifndef TILEMAP_H_
#define TILEMAP_H_

#include <vector>
#include "sprite.h"

class TileMap
{
public:
	TileMap (const char* lvlFile);

	void Draw();
	std::vector<Sprite*> GetMap(){return m_Map;}
	std::vector<Sprite*> GetObjects(){return m_Objects;}

private:
	std::vector<Sprite*> m_Map;
	std::vector<Sprite*> m_Objects;
};
#endif