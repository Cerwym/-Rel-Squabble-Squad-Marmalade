#include <fstream>
#include <string>
#include <iostream>
#include "TileMap.h"
using namespace std;


TileMap::TileMap(const char* lvlFile)
{
	ifstream infile(lvlFile);
	string line;
	int y = 0;
	char buff[1024];
	//char* buff = new char[1024];

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


					if (buff[x] == '9')
					{
						GameObject* t = new GameObject("m_floor", Floor);
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);

						//cout << "Sprite is at -> " << t->GetWidth() * x << "," << t->GetHeight() * y  << endl;
					}

					if (buff[x] == 'X')
					{
						GameObject* t = new GameObject("exit", Exit);
						//t->SetCenter(CIwSVec2(t->GetWidth() / 2, t->GetHeight() /2));
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Objects.push_back(t);
					}
				}
			}
			y++;
			line.clear();
		}
		infile.close();
	}

	// Link up the GameObjects

	AddRelationships();
}

// This is a "temporary" function, ideally I want to be able to create a level format with these definitions built in, but for now i have to hard code it in.
void TileMap::AddRelationships()
{
	m_Objects.at(0)->AddChildObject(m_Objects.at(4)); // Add the first terminal to the first elevator

	m_Objects.at(3)->AddChildObject(m_Objects.at(2)); 	// Add the first door to the first button

	// Update all the colliders with their new positions;

	for (auto it = m_Map.begin(); it != m_Map.end(); ++it)
	{
		// Possibly print out all the locations for each collider to verify they are being created properly
		(*it)->UpdateCollider();
		(*it)->ShowColliderPos = true;
	}

	for (auto it = m_Objects.begin(); it != m_Objects.end(); ++it)
	{
		(*it)->UpdateCollider();
		(*it)->ShowColliderPos = true;
	}
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