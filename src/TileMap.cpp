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
					if (buff[x] == '9')
					{
						Sprite* t = new Sprite("m_floor");
						t->BuildCollision("tiles\\m_floor.png");
						t->SetCenter(CIwSVec2(t->GetWidth() / 2, t->GetHeight() /2));
						t->SetPosition(CIwFVec2(t->GetWidth() * x, t->GetHeight() * y));
						m_Map.push_back(t);

						//cout << "Sprite is at -> " << t->GetWidth() * x << "," << t->GetHeight() * y  << endl;
					}

					if (buff[x] == 'E')
					{
						Sprite* t = new Sprite("elevator");
						t->BuildCollision("tiles\\elevator.png");
						t->SetCenter(CIwSVec2(t->GetWidth() / 2, t->GetHeight() /2));
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

	//delete buff;
}

void TileMap::Draw() // make it aware of cam, if not on screen, don't draw
{
	for (auto it = m_Map.begin(); it != m_Map.end(); ++it)
		(*it)->Draw();

	for (auto it = m_Objects.begin(); it != m_Objects.end(); ++it)
		(*it)->Draw();	
}