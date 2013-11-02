
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
#include "utils.h"

// For me
#define DAVE 0
#define NIGEL 1
#define MANDY 2
#define TIME_TO_WAIT 2.5

CGame::CGame(): m_Position(0,0), m_Size(Iw2DGetSurfaceHeight() / 10, Iw2DGetSurfaceHeight() / 10)
, m_UpdatesPerSec(0), m_TimeToOneSec(0), m_CountUpdates(0)
{
	// Sprites
	IwResManagerInit();
	IwGetResManager()->LoadGroup("sprites.group");

	// Dave (big), Nigel (small), Mandy (girl)
	characters[0] = new Sprite("dave");
	characters[0]->SetCenter(CIwSVec2((int16)characters[0]->GetWidth() /2 , (int16)characters[0]->GetHeight() /2));
	characters[0]->SetPosition(CIwFVec2(0,0));
	characters[0]->BuildCollision("characters\\dave.png");
	characters[0]->ShowColliderPos = true;

	characters[1] = new Sprite("nigel");
	characters[1]->SetCenter(CIwSVec2((int16)characters[1]->GetWidth() /2, (int16)characters[1]->GetHeight() /2));
	characters[1]->SetPosition(CIwFVec2(24, 100));
	characters[1]->BuildCollision("characters\\nigel.png");
	characters[1]->ShowColliderPos = true;

	characters[2] = new Sprite("mandy");
	characters[2]->SetCenter(CIwSVec2((int16)characters[2]->GetWidth() /2, (int16)characters[2]->GetHeight() /2));
	characters[2]->SetPosition(CIwFVec2(64, 150));
	characters[2]->BuildCollision("characters\\mandy.png");
	characters[2]->ShowColliderPos = true;

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

	m_throwingTarget = new Sprite("nigel_port");
	m_throwingTarget->BuildCollision("portraits\\nigel_port.png");
	m_throwingTarget->ShowColliderPos = true;

	// I want to put this in a class, will do later
	m_LastUpdate = s3eTimerGetMs();

	screenHeight = Iw2DGetSurfaceHeight();
	screenWidth = Iw2DGetSurfaceWidth();

	m_Font = Iw2DCreateFontResource("font_small");
	Iw2DSetFont(m_Font);

	TEMP_charIndex = 0;
	TEMP_isThrowing = false;
	m_canThrow = false;

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
			if (m_Portraits[i]->isColliding(CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY())))
			{
				if (TEMP_charIndex != i)
				{
					m_Cam->SetPosition(CIwSVec2(static_cast<int16>(-characters[i]->GetPosition().x + (screenWidth /2)), static_cast<int16>(-characters[i]->GetPosition().y + (screenHeight - characters[i]->GetHeight()))));
					TEMP_charIndex = i; // Set the character to be the element that was collided against
				}
				if (characters[i]->isColliding(CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY())))
					std::cout << "clicked" << std::endl;
			}
		}

		if (n_guiButtons[0]->isColliding((CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY())))) // Left Arrow Button
		{
			characters[TEMP_charIndex]->MoveBy(CIwSVec2(-10, 0));
			m_Cam->MoveBy(CIwSVec2(10,0));
		}

		if (n_guiButtons[1]->isColliding((CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY()))))
		{
			characters[TEMP_charIndex]->MoveBy(CIwSVec2(10, 0));
			m_Cam->MoveBy(CIwSVec2(-10, 0));
		}

		if (characters[DAVE]->isColliding(characters[NIGEL]->GetPosition()) && m_canThrow == false)
		{
			if (characters[NIGEL]->isColliding((CIwFVec2(s3ePointerGetX() - (float)m_Cam->GetPosition().x , s3ePointerGetY() - (float)m_Cam->GetPosition().y ))))
			{
				while (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN)
				{
					// Continue to poll input
					s3ePointerUpdate();
					s3eKeyboardUpdate();
					m_canThrow = true;
					m_throwingTarget->SetPosition((CIwFVec2(s3ePointerGetX() - (float)m_Cam->GetPosition().x , s3ePointerGetY() - (float)m_Cam->GetPosition().y )));
					Render();
				}

				TEMP_isThrowing = true;
				TEMP_charIndex = NIGEL;
			}
		}

		m_MouseClicked = true;
	}
	if (s3ePointerGetState(S3E_POINTER_BUTTON_LEFTMOUSE) == 4)
		m_MouseClicked = false;

	for (int i = 0; i < 3; i++)
	{
		characters[i]->Update(dtSecs);

		for (size_t j = 0; j < m_Level->GetMap().size(); j++)
		{
			Sprite* t = m_Level->GetMap().at(j);

			if (characters[DAVE]->isColliding(t->GetPosition()))
				characters[DAVE]->TEMP_ISCOLLIDING = characters[DAVE]->isColliding(t->GetPosition());
			if (characters[MANDY]->isColliding(t->GetPosition()))
				characters[MANDY]->TEMP_ISCOLLIDING = characters[MANDY]->isColliding(t->GetPosition());

			if (!TEMP_isThrowing)
				if (characters[NIGEL]->isColliding(t->GetPosition()))
				characters[NIGEL]->TEMP_ISCOLLIDING = characters[NIGEL]->isColliding(t->GetPosition());

			/*if (characters[i]->isColliding(t->GetPosition()))
				characters[i]->TEMP_ISCOLLIDING = characters[i]->isColliding(t->GetPosition());*/

		}
	}

	if (TEMP_isThrowing)
	{
		characters[NIGEL]->LerpTo(CIwFVec2(m_throwingTarget->GetPosition().x, m_throwingTarget->GetPosition().y), 0.05f);
		characters[NIGEL]->TEMP_ISFALLING = false;
		//Dirty fix, collision detection is not quite accurate
		float xComp = m_throwingTarget->GetPosition().x - characters[NIGEL]->GetPosition().x;
		float yComp = m_throwingTarget->GetPosition().y - characters[NIGEL]->GetPosition().y;
		// greater than or equal to -0.005, and less than or equal to 0.005

		std::cout << "Difference from 0 in x is " << xComp << "y is" << yComp << std::endl;
		if ((xComp >= -2.05 && xComp <= 2.05) && yComp >= -2.05 && yComp <= 2.05)
		{
			std::cout << "Reached target" << std::endl;
			m_canThrow = false;
			TEMP_isThrowing = false;
		}
		//characters[NIGEL]->TEMP_ISCOLLIDING = false;
	}
	m_Cam->SetPosition(CIwSVec2(static_cast<int16>(-characters[TEMP_charIndex]->GetPosition().x + (screenWidth /2)), static_cast<int16>(-characters[TEMP_charIndex]->GetPosition().y + (screenHeight - characters[TEMP_charIndex]->GetHeight()))));
}

void CGame::DrawText(int16 x, int16 y)
{
	// Draw Nigel's Position
	// Draw Mandy's Position
	// Draw 
	Iw2DDrawString("This is some pointless text", CIwSVec2(x - 100, y - 110), CIwSVec2((int16)screenWidth, (int16)screenHeight),IW_2D_FONT_ALIGN_CENTRE, IW_2D_FONT_ALIGN_CENTRE);
}

void CGame::Render()
{
    Iw2DSurfaceClear(0xff000000);

	Iw2DDrawImage(m_Layer1, CIwSVec2((int16)layerLocs.at(0).x, (int16)layerLocs.at(0).y));
	Iw2DDrawImage(m_Layer2, CIwSVec2((int16)layerLocs.at(1).x, (int16)layerLocs.at(1).y));
	Iw2DDrawImage(m_Layer3, CIwSVec2((int16)layerLocs.at(2).x, (int16)layerLocs.at(2).y));
	m_Level->Draw();

	for (int i = 0;  i <3; i++)
	{
		characters[i]->Draw();
		m_Portraits[i]->Draw(m_Cam->GetPosition());		
	}

	if (m_canThrow)
		m_throwingTarget->Draw();
		
	//DrawTouchButtons();

	for (int i = 0; i < 2; i++)
		n_guiButtons[i]->Draw(m_Cam->GetPosition());

	DrawText(m_Cam->GetPosition().x, m_Cam->GetPosition().y);

    Iw2DSurfaceShow();
}