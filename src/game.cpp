
/*
 * This file is part of the Marmalade SDK Code Samples.
 *
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This source code is intended only as a supplement to the Marmalade SDK.
 *
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 */

#include <vector>
#include <iostream> // cout
#include "s3e.h"
#include "IwGx.h"
#include "Iw2D.h"
#include "IwResManager.h"
#include "game.h"

CGame::CGame(): m_Position(0,0), m_Size(Iw2DGetSurfaceHeight() / 10, Iw2DGetSurfaceHeight() / 10)
, m_UpdatesPerSec(0), m_TimeToOneSec(0), m_CountUpdates(0)
{
	// Sprites
	IwResManagerInit();
	IwGetResManager()->LoadGroup("sprites.group");

	// Dave (big), Nigel (small), Mandy (girl)
	characters[0] = new Sprite("dave");
	characters[0]->SetCenter(CIwSVec2(characters[0]->GetWidth(), characters[0]->GetHeight()));
	characters[0]->SetPosition(CIwFVec2(0,0));
	characters[0]->BuildCollision("characters\\dave.png");

	characters[1] = new Sprite("nigel");
	characters[1]->SetCenter(CIwSVec2(characters[1]->GetWidth(), characters[1]->GetHeight()));
	characters[1]->SetPosition(CIwFVec2(24, 100));
	characters[1]->BuildCollision("characters\\nigel.png");

	characters[2] = new Sprite("mandy");
	characters[2]->SetCenter(CIwSVec2(characters[2]->GetWidth(), characters[2]->GetHeight()));
	characters[2]->SetPosition(CIwFVec2(64, 150));
	characters[2]->BuildCollision("characters\\mandy.png");

	m_Portraits[0] = new Sprite("dave_port");
	m_Portraits[0]->SetPosition(CIwFVec2(130,0));
	m_Portraits[0]->BuildCollision("portraits\\dave_port.png");

	m_Portraits[1] = new Sprite("nigel_port");
	m_Portraits[1]->SetPosition(CIwFVec2(210,0));
	m_Portraits[1]->BuildCollision("portraits\\nigel_port.png");

	m_Portraits[2] = new Sprite("mandy_port");
	m_Portraits[2]->SetPosition(CIwFVec2(290,0));
	m_Portraits[2]->BuildCollision("portraits\\mandy_port.png");

	m_Layer1 = Iw2DCreateImageResource("layer1");
	m_Layer2 = Iw2DCreateImageResource("layer2");
	m_Layer3 = Iw2DCreateImageResource("layer3");
	m_Layer4 = Iw2DCreateImageResource("layer4");

	// Load gui buttons in, 0 = left arrow, 1 right arrow
	guiButtons[0] = Iw2DCreateImageResource("touchscreenMoveL");
	guiButtons[1] = Iw2DCreateImageResource("touchscreenMoveR");

	for (int i = 0; i < 4; i++)
	{
		layerLocs.push_back(CIwFVec2(0,0));
	}

	n_guiButtons[0] = new Sprite("touchScreenMoveL");
	n_guiButtons[0]->SetPosition(CIwFVec2(0, 260));
	n_guiButtons[0]->BuildCollision("textures\\touchScreenMoveL.bmp");

	n_guiButtons[1] = new Sprite("touchScreenMoveR");
	n_guiButtons[1]->SetPosition(CIwFVec2(414, 260));
	n_guiButtons[1]->BuildCollision("textures\\touchScreenMoveR.bmp");

	// I want to put this in a class, will do later
	m_LastUpdate = s3eTimerGetMs();

	screenHeight = Iw2DGetSurfaceHeight();
	screenWidth = Iw2DGetSurfaceWidth();

	m_Font = Iw2DCreateFontResource("font_small");
	Iw2DSetFont(m_Font);

	TEMP_charIndex = 0;
	TEMP_isThrowing = false;
	TEMP_canThrow = false;

	m_Cam = new Camera;
	m_Cam->SetPosition(CIwSVec2(0, 0));
	m_Cam->Position = CIwSVec2(0, 0);
	
	m_Level = new TileMap("levels\\levelproto.txt");
	m_MouseClicked = false;
}


CGame::~CGame()
{
	for (int i = 0; i < 3; i++)
	{
		delete characters[i];
		delete m_Portraits[i];
	}

	for (int i = 0; i < 2; i++)
		delete n_guiButtons[i];

	if (m_Font != NULL)
		delete m_Font;
	
	if (m_Layer1 != NULL)
		delete m_Layer1;
	if (m_Layer2 != NULL)
		delete m_Layer2;
	if (m_Layer3 != NULL)
		delete m_Layer3;
	if (m_Layer4 != NULL)
		delete m_Layer4;

	if (m_Floor != NULL)
		delete m_Floor;

	delete m_Cam;
	IwResManagerTerminate();
}


void CGame::Update()
{
    // game logic goes here
	uint64 deltaTime = s3eTimerGetMs() - m_LastUpdate;
	m_LastUpdate = s3eTimerGetMs();
	float dtSecs = deltaTime * 0.001f;

	m_TimeToOneSec += (int)deltaTime;

	if (m_TimeToOneSec >= 1000) 
	{ 
		m_TimeToOneSec -= 1000; 
		m_UpdatesPerSec = m_CountUpdates;
		m_CountUpdates = 0;
	}

	m_CountUpdates++;

	// Input

    if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
    {
		for (int i = 0; i < 3; i++)
		{
			// Check collision with the character portraits
			if (m_Portraits[i]->isColliding(CIwFVec2(s3ePointerGetX(), s3ePointerGetY())))
			{
				if (TEMP_charIndex != i)
				{
					m_Cam->SetPosition(CIwSVec2(static_cast<int16>(-characters[i]->GetPosition().x + (screenWidth /2)), static_cast<int16>(-characters[i]->GetPosition().y + (screenHeight - characters[i]->GetHeight()))));
					//TEMP_charIndex = i;
				}
				if (characters[i]->isColliding(CIwFVec2(s3ePointerGetX(), s3ePointerGetY())))
					std::cout << "clicked" << std::endl;
			}

			if (characters[i]->isColliding(CIwFVec2(s3ePointerGetX() - m_Cam->GetPosition().x , s3ePointerGetY() - m_Cam->GetPosition().y )))
				std::cout << "Clicked on " << i << std::endl;
		}

		// Check if Nigel can be thrown, if he can, check to see if the player has pressed on Nigel, if he has, then begin throwing.
		if (TEMP_canThrow)
			TEMP_isThrowing = characters[1]->isColliding(CIwFVec2(s3ePointerGetX() - m_Cam->GetPosition().x, s3ePointerGetY() - m_Cam->GetPosition().y ));

		if (!TEMP_isThrowing)
		{
			if (n_guiButtons[0]->isColliding((CIwFVec2(s3ePointerGetX(), s3ePointerGetY())))) // Left Arrow Button
			{
				characters[TEMP_charIndex]->MoveBy(CIwSVec2(-10, 0));
				m_Cam->MoveBy(CIwSVec2(10,0));
			}

			if (n_guiButtons[1]->isColliding((CIwFVec2(s3ePointerGetX(), s3ePointerGetY()))))
			{
				characters[TEMP_charIndex]->MoveBy(CIwSVec2(10, 0));
				m_Cam->MoveBy(CIwSVec2(-10, 0));
			}
		}
		else
		{
			TEMP_target = CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY());
		}
		m_MouseClicked = true;
	}
	if (s3ePointerGetState(S3E_POINTER_BUTTON_LEFTMOUSE) == 4)
		m_MouseClicked = false;



	if (TEMP_isThrowing == true)
	{
		characters[1]->SetPosition(characters[1]->LerpTo(TEMP_target, 0.05f));
		characters[1]->TEMP_ISFALLING = true;
	}

	for (int i = 0; i < 3; i++)
	{
		characters[i]->Update(dtSecs);

		for (int j = 0; j < m_Level->GetMap().size(); j++)
		{
			Sprite* t = m_Level->GetMap().at(j);
			if (characters[i]->isColliding(t->GetPosition()))
				characters[i]->TEMP_ISCOLLIDING = true;

			if (characters[1]->isColliding(t->GetPosition()))
				TEMP_isThrowing = false;
		}
	}

	// If Dave is colliding with Nigel, then make it so
	TEMP_canThrow = characters[0]->isColliding((characters[1]->GetPosition()));

	m_Cam->SetPosition(CIwSVec2(static_cast<int16>(-characters[TEMP_charIndex]->GetPosition().x + (screenWidth /2)), static_cast<int16>(-characters[TEMP_charIndex]->GetPosition().y + (screenHeight - characters[TEMP_charIndex]->GetHeight()))));
}

void CGame::DrawText()
{
	// Draw Nigel's Position
	// Draw Mandy's Position
	// Draw 
}

void CGame::Render()
{
    Iw2DSurfaceClear(0xff000000);

	Iw2DDrawImage(m_Layer1, CIwSVec2(layerLocs.at(0).x, layerLocs.at(0).y));
	Iw2DDrawImage(m_Layer2, CIwSVec2(layerLocs.at(1).x, layerLocs.at(1).y));
	Iw2DDrawImage(m_Layer3, CIwSVec2(layerLocs.at(2).x, layerLocs.at(2).y));
	m_Level->Draw();

	for (int i = 0;  i <3; i++)
	{
		characters[i]->Draw();
		m_Portraits[i]->Draw(m_Cam->GetPosition());		
	}
	
	//DrawTouchButtons();

	for (int i = 0; i < 2; i++)
		n_guiButtons[i]->Draw(m_Cam->GetPosition());

    Iw2DSurfaceShow();
}

void CGame::DrawTouchButtons()
{
	int size = MIN(screenWidth/3, screenHeight/4) - 8;
	if (size < 64)
		size = 32;
	else
		size = 64;

	Iw2DSetColour(0xff646464);
	Iw2DSetAlphaMode(IW_2D_ALPHA_ADD);
	DrawSpriteCentered(guiButtons[0], size/2 - m_Cam->GetPosition().x, screenHeight-size/2 - m_Cam->GetPosition().y, size); // Left side
	DrawSpriteCentered(guiButtons[1], screenWidth-size/2 - m_Cam->GetPosition().x, screenHeight-size/2 - m_Cam->GetPosition().y, size);
	Iw2DSetAlphaMode(IW_2D_ALPHA_NONE);
	Iw2DSetColour(0xffffffff);
}

void CGame::DrawSpriteCentered(CIw2DImage* img, int x, int y, int size)
{
	Iw2DDrawImage(img, CIwSVec2(x, y) - CIwSVec2(size/2, size/2), CIwSVec2(size, size));
}