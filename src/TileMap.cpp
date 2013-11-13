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

					/*

					E- elevator
					B - button
					T - terminal
					D - door
					1 - floor_left_1
					2 - floor_repeatable_2
					3 - floor_right_3
					4 - wall_top_4
					5 - wall_repeatable_5
					6 - wall_bottom_6
					7 - vent_opening_7
					8 - vent_repeatable_8
					9 - vent_corner_9
					s - vent_support
					0 - vent_up_0
					o - vent_o
					p - vent_p

					*/
					if (buff[x] == 'E')
					{
						GameObject* t = new GameObject("elevator", Elevator);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Objects.push_back(t);
					}

					if (buff[x] == 'B')
					{
						GameObject* t = new GameObject("button", Button);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Objects.push_back(t);
					}
					
					if (buff[x] == 'D')
					{
						GameObject* t = new GameObject("door", Door);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Objects.push_back(t);
					}

					if (buff[x] == 'T')
					{
						GameObject* t = new GameObject("terminal", Terminal);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Objects.push_back(t);
					}

					if (buff[x] == 'X')
					{
						GameObject* t = new GameObject("exit", Exit);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Objects.push_back(t);
					}

					// Map Data

					if (buff[x] == '1')
					{
						GameObject* t = new GameObject("floor_left_1", Scenerary);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '2')
					{
						GameObject* t = new GameObject("floor_repeatable_2", Scenerary);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '3')
					{
						GameObject* t = new GameObject("floor_right_3", Scenerary);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '4')
					{
						GameObject* t = new GameObject("wall_top_4", Scenerary);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '5')
					{
						GameObject* t = new GameObject("wall_repeatable_5", Scenerary);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '6')
					{
						GameObject* t = new GameObject("wall_bottom_6", Scenerary);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '7')
					{
						GameObject* t = new GameObject("vent_opening_7", Scenerary);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '8')
					{
						GameObject* t = new GameObject("vent_repeatable_8", Scenerary);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == '9')
					{
						GameObject* t = new GameObject("vent_corner_9", Scenerary);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == 's')
					{
						GameObject* t = new GameObject("vent_support", Scenerary);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, (y * 32) - t->GetHeight()));
						m_Map.push_back(t);
					}

					if (buff[x] == '0')
					{
						GameObject* t = new GameObject("vent_up_0", Scenerary);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == 'o')
					{
						GameObject* t = new GameObject("vent_o", Scenerary);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);
					}

					if (buff[x] == 'p')
					{
						GameObject* t = new GameObject("vent_p", Scenerary);
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

			std::cout << "Element in " << y << " is " << relationships.at(y).x << "," << relationships.at(y).y << std::endl;
			y++;
			line.clear();
		}
		infile.close();
	}

	for (int i = 0; i < relationships.size(); i++)
	{
		std::cout << "Relationship " << i << " ->" << relationships.at(i).x << "," << relationships.at(i).y << std::endl;
	}

	for (auto it = relationships.begin(); it != relationships.end(); ++it)
		m_Objects.at(it->x)->AddChildObject(m_Objects.at(it->y));

	//end
/*
	m_Objects.at(0)->AddChildObject(m_Objects.at(5)); 
	m_Objects.at(4)->AddChildObject(m_Objects.at(3));
	m_Objects.at(2)->AddChildObject(m_Objects.at(9));
	m_Objects.at(6)->AddChildObject(m_Objects.at(8));
	m_Objects.at(7)->AddChildObject(m_Objects.at(8));
*/

	for (auto it = m_Map.begin(); it != m_Map.end(); ++it)
	{
		// Possibly print out all the locations for each collider to verify they are being created properly
		(*it)->UpdateCollider();
		//(*it)->ShowColliderPos = true;
	}

	for (auto it = m_Objects.begin(); it != m_Objects.end(); ++it)
	{
		(*it)->UpdateCollider();
		(*it)->ShowColliderPos = true;
	}

	std::cout << "Relationships added" << std::endl;
}

void TileMap::Draw() // make it aware of cam, if not on screen, don't draw
{
	for (auto it = m_Map.begin(); it != m_Map.end(); ++it)
		(*it)->Draw();

	for (auto it = m_Objects.begin(); it != m_Objects.end(); ++it)
	{
		if ((*it)->IsActive == true)
		{
			(*it)->Draw();	
		}
		else
		{
			std::cout << "Not drawing something" << std::endl;
		}
	}
}