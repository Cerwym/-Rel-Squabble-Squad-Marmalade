#include <fstream>
#include <string>
#include <iostream>
#include "TileMap.h"
using namespace std;


TileMap::TileMap(const char* lvlFile, const char* rFile)
{
	// Initialize the vector with 3 elements (0,0) so that we can add the character's parsed positions in game
	for (int i = 0; i <3; i++)
		m_SpawnPos.push_back(CIwFVec2(0,0));

	ifstream infile(lvlFile);
	string line;
	int y = 0;
	char buff[1024];

	// Throw an assert if the texture is NOT equal to TILE_WIDTH or TILE_HEIGHT

	if ((!infile.bad()) || (infile != NULL))
	{
		while(std::getline(infile, line) != NULL)
		{
			strncpy(buff, line.c_str(), sizeof(buff));
			buff[sizeof(buff) - 1] = 0;

			for (size_t x = 0; x < line.length(); x++)
			{
				if( (buff[x] != '\r') || (buff[x] != '*'))
				{

					// graffiti tiles
					if (buff[x] == 'g')
					{
						GameObject* t = new GameObject("graff_1", Scenerary, false);
						t->SetPosition(CIwFVec2(((x * 32)), (y * 32)));
						m_Map.push_back(t);
					}

					if (buff[x] == 'h')
					{
						GameObject* t = new GameObject("graff_2", Scenerary, false);
						t->SetPosition(CIwFVec2(((x * 32)), (y * 32)));
						m_Map.push_back(t);
					}

					if (buff[x] == 'j')
					{
						GameObject* t = new GameObject("graff_3", Scenerary, false);
						t->SetPosition(CIwFVec2(((x * 32)), (y * 32)));
						m_Map.push_back(t);
					}

					if (buff[x] == 'k')
					{
						GameObject* t = new GameObject("graff_4", Scenerary, false);
						t->SetPosition(CIwFVec2(((x * 32)), (y * 32)));
						m_Map.push_back(t);
					}

					if (buff[x] == 'l')
					{
						GameObject* t = new GameObject("graff_5", Scenerary, false);
						t->SetPosition(CIwFVec2(((x * 32)), (y * 32)));
						m_Map.push_back(t);
					}

					if (buff[x] == 'E')
					{
						GameObject* t = new GameObject("elevator", Elevator, true);
						t->SetPosition(CIwFVec2(((x * 32)), (y * 32) + 32)); // its width
						// For the elevator we will need to store the location where it spawns because of logic that makes it raise and lower.
						t->SetStartPosition(t->GetPosition());
						m_Objects.push_back(t);
					}

					if (buff[x] == 'B')
					{
						GameObject* t = new GameObject("button", Button, true);
						t->SetPosition(CIwFVec2(((x * 32) + 32) - t->GetWidth(), (y * 32) + 20));
						m_Objects.push_back(t);
					}
					
					if (buff[x] == 'D')
					{
						GameObject* t = new GameObject("door", Door, true);
						t->SetPosition(CIwFVec2(((x * 32) + 32) - t->GetWidth(), (y * 32)));
						m_Objects.push_back(t);
					}

					if (buff[x] == 'T')
					{
						GameObject* t = new GameObject("terminal", Terminal, true);
						t->SetPosition(CIwFVec2(((x * 32) + 32) - t->GetWidth(), (y * 32)));
						t->IsActive = false;
						m_Objects.push_back(t);
					}

					if (buff[x] == 'X')
					{
						GameObject* t = new GameObject("exit", Exit, true);
						t->SetPosition(CIwFVec2(((x * 32) + 32) - t->GetWidth(), t->GetHeight() * y));
						m_Objects.push_back(t);
					}

					// Map Data

					if (buff[x] == '1')
					{
						GameObject* t = new GameObject("floor_left_1", Scenerary, true);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '2')
					{
						GameObject* t = new GameObject("floor_repeatable_2", Scenerary, true);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '3')
					{
						GameObject* t = new GameObject("floor_right_3", Scenerary, true);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '4')
					{
						GameObject* t = new GameObject("wall_top_4", Scenerary, true);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '5')
					{
						GameObject* t = new GameObject("wall_repeatable_5", Scenerary, true);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '6')
					{
						GameObject* t = new GameObject("wall_bottom_6", Scenerary, true);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '7')
					{
						GameObject* t = new GameObject("vent_opening_7", Scenerary, true);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '8')
					{
						GameObject* t = new GameObject("vent_repeatable_8", Scenerary, true);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '9')
					{
						GameObject* t = new GameObject("vent_corner_9", Scenerary, true);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == 's')
					{
						GameObject* t = new GameObject("vent_support", Scenerary, true);
						t->SetPosition(CIwFVec2((x * 32), ((y*32) + 32) - t->GetHeight())); // check if this is drawn in the right way
						m_Map.push_back(t);
					}

					if (buff[x] == '0')
					{
						GameObject* t = new GameObject("vent_up_0", Scenerary, true);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == 'o')
					{
						GameObject* t = new GameObject("vent_o", Scenerary, false);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == 'p')
					{
						GameObject* t = new GameObject("vent_p", Scenerary, false);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					// Characters
					if (buff[x] == 'd')
						m_SpawnPos.at(0) = CIwFVec2(x * 32,(y * 32) - 100); // offset by sprite's height
					if (buff[x] == 'n')
						m_SpawnPos.at(1) = CIwFVec2(x * 32,(y * 32) - 46); // offset by sprite's height
					if (buff[x] == 'm')
						m_SpawnPos.at(2) = CIwFVec2(x * 32,(y * 32) - 69); // offset by sprite's height
				}
			}
			y++;
			line.clear();
		}
		infile.close();
	}

	// Link up the GameObjects
	printf("Level %s loaded\n", lvlFile);
	AddRelationships(rFile);
}

TileMap::~TileMap()
{
	for (auto it = m_Map.begin(); it != m_Map.end(); ++it)
		delete (*it);

	for (auto it = m_Objects.begin(); it != m_Objects.end(); ++it)
		delete (*it);
}

// This is a "temporary" function, ideally I want to be able to create a level format with these definitions built in, but for now i have to hard code it in.
void TileMap::AddRelationships(const char* rFile)
{
	IW_CALLSTACK("TileMap::AddRelationships");

	std::vector<CIwSVec2> relationships;

	ifstream infile(rFile);
	string line;
	int y = 0;
	char buff[4];
	if ((!infile.bad()) || (infile != NULL))
	{
		while(std::getline(infile, line) != NULL)
		{
			strncpy(buff, line.c_str(), sizeof(buff));
			buff[sizeof(buff) - 1] = 0;

			relationships.push_back(CIwSVec2(0,0));

			std::cout << "line ->" << line;

			for (size_t x = 0; x < line.length(); x++)
			{
				if( (buff[x] != '\r'))
				{
					if (x == 0)
						relationships.back().x = buff[x] - '0';
					if (x == 1)
						relationships.back().y = buff[x] - '0';
				}
				std::cout << buff[x];
			}
			y++;
			line.clear();
		}
		infile.close();
	}

	for (auto it = relationships.begin(); it != relationships.end(); ++it)
		m_Objects.at(it->x)->AddChildObject(m_Objects.at(it->y));

	for (auto it = m_Map.begin(); it != m_Map.end(); ++it)
	{
		if ( (*it)->hasCollider())
		{
			(*it)->UpdateCollider();
		}
		//(*it)->ShowColliderPos = true;
	}

	for (auto it = m_Objects.begin(); it != m_Objects.end(); ++it)
	{
		if ( (*it)->hasCollider())
		{
			(*it)->UpdateCollider();
			//(*it)->ShowColliderPos = true;
		}
	}

	std::cout << "Relationships added" << std::endl;
}

void TileMap::Draw(Camera* cam) // make it aware of cam, if not on screen, don't draw
{
	for (auto it = m_Map.begin(); it != m_Map.end(); ++it)
	{
		// If it is on camera, draw it
		(*it)->Draw();
	}

	for (auto it = m_Objects.begin(); it != m_Objects.end(); ++it)
	{
		if ((*it)->GetType() == Door)
		{
			if ((*it)->IsActive)
				(*it)->Draw();
		}
		else
			(*it)->Draw();	
	}
}