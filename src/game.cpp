
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

	// The background image does NOT take from Sprite:: as the drawing method is different
	bgImg = Iw2DCreateImageResource("background"); 

	m_Pig = new Sprite("fireball");
	m_Pig->SetCenter(CIwSVec2(50,50));

	m_Layer1 = Iw2DCreateImageResource("layer1");
	m_Layer2 = Iw2DCreateImageResource("layer2");
	m_Layer3 = Iw2DCreateImageResource("layer3");
	m_Layer4 = Iw2DCreateImageResource("layer4");

	// Load gui buttons in, 0 = left arrow, 1 right arrow
	guiButtons[0] = Iw2DCreateImageResource("touchscreenMoveL");
	guiButtons[1] = Iw2DCreateImageResource("touchscreenMoveR");

	for (int i = 0; i < 9; i++)
	{
		Sprite *tSprite = new Sprite("fireball");
		tSprite->SetCenter(CIwSVec2(50,50));
		tSprite->SetPosition(CIwFVec2( (i*20) +50, (i*20) + 50));
		m_Sprites.push_back(tSprite);
	}

	for (int i = 0; i < 4; i++)
	{
		layerLocs.push_back(CIwFVec2(0,0));
	}

	// I want to put this in a class, will do later
	m_LastUpdate = s3eTimerGetMs();

	screenHeight = Iw2DGetSurfaceHeight();
	screenWidth = Iw2DGetSurfaceWidth();

	m_Font = Iw2DCreateFontResource("font_small");
	Iw2DSetFont(m_Font);
}


CGame::~CGame()
{
	if (m_Pig != NULL)
		delete m_Pig;

	for (size_t i = 0; i < m_Sprites.size(); i++)
		if (m_Sprites.at(i) != NULL)
		{
			delete m_Sprites.at(i);
			m_Sprites.erase(m_Sprites.begin() + i);
		}

	for (int i = 0; i < 2; i++)
		delete guiButtons[i];

	if (bgImg != NULL)
		delete bgImg;

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
    if( s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN )
    {
        std::cout << "Pointer @ x:" << s3ePointerGetX() << " y:" << s3ePointerGetY() << std::endl;
		int x = s3ePointerGetX() * 3 / screenWidth;
		int y = s3ePointerGetY() * 4 / screenHeight;

		std::cout << "Pointer @ x:" << x << " y:" << y << std::endl;
		
		if (x == 0 && y == 3)
		{ //Pressed the left button
			m_Position.x -=5;
			layerLocs.at(0).x += 0.25;
			layerLocs.at(1).x += 0.5;
			layerLocs.at(2).x += 0.75;
			layerLocs.at(3).x += 0.75;
		}
		else if (x == 2 && y == 3)
		{//Pressed the right button
			m_Position.x +=5;
			layerLocs.at(0).x -= 0.25;
			layerLocs.at(1).x -= 0.5;
			layerLocs.at(2).x -= 0.75;
			layerLocs.at(3).x -= 0.75;
		}
		else // Re sample the position of the touch event as no 'button' was pressed
		{
			// Move the sprite to the position of a touch event, gradually
			CIwFVec2 target((float)s3ePointerGetX(), (float)s3ePointerGetY());
			//m_Position += (target - m_Position) * 5 * dtSecs;

			
		}
		//m_Pig->SetPosition(m_Position);

		m_Pig->SetPosition(m_Pig->LerpTo(CIwFVec2(480, 320), 0.005f));
	}

	m_Pig->Update(dtSecs);

	// loop over all the pig sprites and see if the hero pig collides
	for (size_t i = 0; i < m_Sprites.size(); i++)
	{
		if (m_Pig->isColliding(m_Sprites.at(i)))
		{
			delete m_Sprites.at(i);
			m_Sprites.erase(m_Sprites.begin() + i);			
		}
	}
}


void CGame::Render()
{
    // for example, clear to black (the order of components is ABGR)
    Iw2DSurfaceClear(0xff000000);

	//DrawBackGround(bgImg, 0, 0, screenWidth, screenHeight);
		
	Iw2DDrawImage(m_Layer1, CIwSVec2(layerLocs.at(0).x, layerLocs.at(0).y));
	Iw2DDrawImage(m_Layer2, CIwSVec2(layerLocs.at(1).x, layerLocs.at(1).y));
	Iw2DDrawImage(m_Layer3, CIwSVec2(layerLocs.at(2).x, layerLocs.at(2).y));

	m_Pig->Draw(); // The last drawn element will be on top

	// Draw message (centered and automatically word wrapped)
	Iw2DDrawString("This is some pointless text", CIwSVec2(-225,-150), CIwSVec2((int16)screenWidth, (int16)screenHeight),
		IW_2D_FONT_ALIGN_CENTRE, IW_2D_FONT_ALIGN_CENTRE);

	for (size_t i = 0; i < m_Sprites.size(); i++)
		m_Sprites.at(i)->Draw();
	
	DrawTouchButtons();

	Iw2DDrawImage(m_Layer4, CIwSVec2(layerLocs.at(3).x, layerLocs.at(3).y));
    // show the surface
    Iw2DSurfaceShow();
}

void CGame::DrawBackGround(CIw2DImage* img, int x0, int y0, int w, int h)
{
	// Get size of image
	int img_width = img->GetWidth();
	int img_height = img->GetHeight();

	// Draw textured tiles
	for (int x = x0; x < w; x+= img_width)
		for (int y = y0; y < h; y+= img_height)
			Iw2DDrawImage(img, CIwSVec2(x, y));
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
	DrawSpriteCentered(guiButtons[0], size/2, screenHeight-size/2, size); // Left side
	DrawSpriteCentered(guiButtons[1], screenWidth-size/2, screenHeight-size/2, size);
	Iw2DSetAlphaMode(IW_2D_ALPHA_NONE);
	Iw2DSetColour(0xffffffff);
}

void CGame::DrawSpriteCentered(CIw2DImage* img, int x, int y, int size)
{
	Iw2DDrawImage(img, CIwSVec2(x, y) - CIwSVec2(size/2, size/2), CIwSVec2(size, size));
}