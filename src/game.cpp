
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
	characters[0]->SetPosition(CIwFVec2(16,0));
	characters[0]->ShowColliderPos = true;

	characters[1] = new Sprite("nigel");
	characters[1]->SetCenter(CIwSVec2((int16)characters[1]->GetWidth() /2, (int16)characters[1]->GetHeight() /2));
	characters[1]->SetPosition(CIwFVec2(24, 100));
	characters[1]->ShowColliderPos = true;

	characters[2] = new Sprite("mandy");
	characters[2]->SetCenter(CIwSVec2((int16)characters[2]->GetWidth() /2, (int16)characters[2]->GetHeight() /2));
	characters[2]->SetPosition(CIwFVec2(64, 150));
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

	n_guiButtons[0] = new Sprite("touchScreenMoveL");
	n_guiButtons[0]->SetPosition(CIwFVec2(0, 260));
	n_guiButtons[0]->BuildCollision("textures\\touchScreenMoveL.bmp");

	n_guiButtons[1] = new Sprite("touchScreenMoveR");
	n_guiButtons[1]->SetPosition(CIwFVec2(414, 260));
	n_guiButtons[1]->BuildCollision("textures\\touchScreenMoveR.bmp");

	m_throwingTarget = new Sprite("nigel_port");
	m_throwingTarget->ShowColliderPos = true;

	// I want to put this in a class, will do later
	m_LastUpdate = s3eTimerGetMs();

	screenHeight = Iw2DGetSurfaceHeight();
	screenWidth = Iw2DGetSurfaceWidth();

	m_Font = Iw2DCreateFontResource("font_small");
	Iw2DSetFont(m_Font);

	TEMP_charIndex = 0;
	TEMP_isThrowing = false;
	TEMP_termActive = false;
	m_canThrow = false;
	m_SpacePressed = false;
	m_UpPressed = false;

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

	/*
	Move this into an input class
	*/

	/*
	if ( (s3eKeyboardGetState(s3eKeySpace) & S3E_POINTER_STATE_DOWN) && m_SpacePressed == false)
	{
		std::cout << "Space pressed" << std::endl;
		if (TEMP_charIndex == MANDY){
			characters[MANDY]->TEMP_JUSTJUMPED = true;}
		m_SpacePressed = true;
	}

	if ( (s3eKeyboardGetState(s3eKeyUp) & S3E_POINTER_STATE_DOWN) && m_UpPressed == false)
	{
		std::cout << "Up pressed" << std::endl;
		m_UpPressed = true;
	}
	
	// Input

	if( (s3ePointerGetState(S3E_POINTER_BUTTON_RIGHTMOUSE) & S3E_POINTER_STATE_DOWN))
	{
		characters[NIGEL]->Debug_PrintPos();
	}

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
				{
					//
				}
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


		if (characters[DAVE]->isColliding(characters[NIGEL]->GetPosition()) && (m_canThrow == false) && TEMP_charIndex == DAVE)
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
					m_throwingTarget->UpdateCollider();
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

	if (TEMP_isThrowing)
	{
		characters[NIGEL]->LerpTo(CIwFVec2(m_throwingTarget->GetPosition().x, m_throwingTarget->GetPosition().y), 0.05f);
		characters[NIGEL]->TEMP_ISFALLING = false;
		characters[NIGEL]->UpdateCollider();
		//Dirty fix, collision detection is not quite accurate
		float xComp = m_throwingTarget->GetPosition().x - characters[NIGEL]->GetPosition().x;
		float yComp = m_throwingTarget->GetPosition().y - characters[NIGEL]->GetPosition().y;
	
		/*if ((xComp >= -5.05 && xComp <= 5.05) && yComp >= -5.05 && yComp <= 5.05)
		{
			std::cout << "Reached target" << std::endl;
			m_canThrow = false;
			TEMP_isThrowing = false;
			characters[NIGEL]->TEMP_ISFALLING = true;
			characters[NIGEL]->TEMP_ISCOLLIDING = false;
		}

		if (characters[NIGEL]->isColliding(m_throwingTarget))
		{
			m_canThrow = false;
			TEMP_isThrowing = false;
			characters[NIGEL]->TEMP_ISFALLING = true;
			characters[NIGEL]->TEMP_ISCOLLIDING = false;
		}
	}

	// Temporary fix
	if (characters[NIGEL]->GetPosition().x >= 950 && characters[NIGEL]->GetPosition().x <= 1034)
	{
		if (characters[NIGEL]->GetPosition().y >= 244.5 && characters[NIGEL]->GetPosition().y <= 266.5)
		{
			std::cout << "hit the gap" << std::endl;
			characters[NIGEL]->TEMP_ISCOLLIDING = false;
		}
	}

	*/

	CheckInterations();

	for (int i = 0; i < 3; i++)
	{
		characters[i]->Update(dtSecs);
		characters[i]->UpdateCollider();
	}
	
	m_Cam->SetPosition(CIwSVec2(static_cast<int16>(-characters[TEMP_charIndex]->GetPosition().x + (screenWidth /2)), static_cast<int16>(-characters[TEMP_charIndex]->GetPosition().y + (screenHeight - characters[TEMP_charIndex]->GetHeight()))));
	
	if (s3eKeyboardGetState(s3eKeyUp) == 4)
		m_UpPressed = false;
	if (s3eKeyboardGetState(s3eKeySpace) == 4)
		m_SpacePressed = false;
}

void CGame::CheckInterations()
{
	int count = 0;

	for (size_t s = 0; s < m_Level->GetMap().size(); s++)
	{
		bool value = m_Level->GetMap().at(s)->isColliding(characters[DAVE]);

		if (value == true)
		{
			std::cout << "Colliding with something" << std::endl;
			characters[DAVE]->TEMP_ISCOLLIDING = true;

			// do an for int = characters, if val = true, break.
		}
	}

	for (size_t s = 0; s < m_Level->GetObjects().size(); s++)
	{
		GameObject *t = m_Level->GetObjects().at(s);
		t->UpdateCollider();

		if (characters[DAVE]->isColliding(t))
			std::cout << "NEW COLLISION ON OBJ" << std::endl;
		// Test if any of the characters hit this object
		for (int i = 0; i < 3; i++)
		{
			if (t->GetType() == Button)
			{
				if (characters[i]->isColliding(t))
				{
					t->Child()->IsActive = false;
					count++;
				}
				else
				{
					if (count == 0)
						t->Child()->IsActive = true;
				}
			}
		}

		if (t->GetType() == Terminal)
		{
			if (characters[MANDY]->isColliding(t))
			{
				if (TEMP_charIndex == MANDY){
					std::cout << "Mandy is Colliding with terminal" << std::endl;}
				TEMP_termActive = true;
				m_activeTerminal = t;
			}
			else
			{
				TEMP_termActive = false;
			}
			//std::cout << "Terminal is " << TEMP_termActive << std::endl;
		}
	}

	// REMOVE THIS
	if (TEMP_termActive)
	{
		if (characters[NIGEL]->isColliding(m_activeTerminal->Child()))
		{
			std::cout << "Nigel -> ";characters[NIGEL]->Debug_PrintPos();
		}
		if (characters[DAVE]->isColliding(m_activeTerminal->Child()))
		{
			std::cout << "DAVE is colliding" << std::endl;
		}
	}


	if (TEMP_charIndex == MANDY)
	{
		if (TEMP_termActive)
		{
			if (m_UpPressed == true)
			{
				if (characters[DAVE]->isColliding(m_activeTerminal->Child()))
				{
					std::cout << "DAVE is colliding" << std::endl;
					characters[DAVE]->SetPosition(CIwFVec2(m_activeTerminal->Child()->GetPosition().x, characters[DAVE]->GetPosition().y - 150));
				}

				std::cout << "Nigel -> ";characters[NIGEL]->Debug_PrintPos();
				std::cout << "Lift -> ";m_activeTerminal->Child()->Debug_PrintPos();
				if (characters[NIGEL]->isColliding(m_activeTerminal->Child())) // +8
				{
					std::cout << "Nigel is colliding" << std::endl;
					characters[NIGEL]->SetPosition(CIwFVec2(m_activeTerminal->Child()->GetPosition().x, characters[NIGEL]->GetPosition().y - 135));
				}
				
				m_activeTerminal->Child()->SetPosition(CIwFVec2(m_activeTerminal->Child()->GetPosition().x, characters[MANDY]->GetPosition().y + 54));
				m_UpPressed = false;
			}
		}
	}
}

void CGame::Render()
{
    Iw2DSurfaceClear(0xff000000);

	m_Level->Draw();

	for (int i = 0; i <3; i++)
	{
		characters[i]->Draw();
		m_Portraits[i]->Draw(m_Cam->GetPosition());		
	}

	if (m_canThrow)
		m_throwingTarget->Draw();

	for (int i = 0; i < 2; i++)
		n_guiButtons[i]->Draw(m_Cam->GetPosition());

    Iw2DSurfaceShow();
}