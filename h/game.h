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
#ifndef GAME_H
#define GAME_H

#include <vector>
#include "sprite.h"

class CGame
{
public:
    CGame();
    ~CGame();

    // update will be called a fixed number of times per second 
    // regardless of visual framerate
    void Update();
    // render will be called as fast as possible (but not faster 
    // than the update rate)
    void Render();

	void DrawBackGround(CIw2DImage* img, int x0, int y0, int w, int h);

protected:
	void DrawTouchButtons();
	// Draw a sprite, centered at the specified position with a given size
	void DrawSpriteCentered(CIw2DImage*, int, int, int);

private:
    CIwFVec2 m_Position;
    CIwSVec2 m_Size;
	Sprite *m_Pig;
	Sprite *m_Target;

	CIw2DImage* bgImg;

	uint64 m_LastUpdate;

	int m_TimeToOneSec;
	int m_CountUpdates;
	int m_UpdatesPerSec;

	int screenWidth;
	int screenHeight;
	
	std::vector<Sprite*> m_Sprites;

	CIw2DFont* m_Font;

	// Temporary stuff
	CIw2DImage* m_Layer1;
	CIw2DImage* m_Layer2;
	CIw2DImage* m_Layer3;
	CIw2DImage* m_Layer4;
	CIw2DImage* guiButtons[2];

	std::vector<CIwFVec2> layerLocs;

};

#endif
